//
// Created by souvik on 11/1/25.
//
#include "DiffStrategy.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "Folder.hpp"

namespace {
void collectNodes(const IFileSystemItem* node,
                  const std::string& path,
                  std::map<std::string, std::string>& files,
                  std::set<std::string>& directories) {
    if (!node) {
        return;
    }

    if (const auto* folder = dynamic_cast<const Folder*>(node)) {
        if (!path.empty()) {
            directories.insert(path + "/");
        }
        for (const auto* child : folder->getItems()) {
            const std::string childPath = path.empty()
                                               ? child->getName()
                                               : path + "/" + child->getName();
            collectNodes(child, childPath, files, directories);
        }
        return;
    }

    const std::string filePath = path.empty() ? node->getName() : path;
    files[filePath] = node->getContent();
}
}

std::string TreeDiffStrategy::diff(const std::shared_ptr<IFileSystemItem>& a,
                                   const std::shared_ptr<IFileSystemItem>& b) {
    std::map<std::string, std::string> filesA;
    std::map<std::string, std::string> filesB;
    std::set<std::string> dirsA;
    std::set<std::string> dirsB;

    collectNodes(a.get(), "", filesA, dirsA);
    collectNodes(b.get(), "", filesB, dirsB);

    std::set<std::string> removed;
    std::set<std::string> added;
    std::set<std::string> modified;
    std::set<std::string> removedDirs;
    std::set<std::string> addedDirs;

    for (const auto& [path, contentA] : filesA) {
        auto it = filesB.find(path);
        if (it == filesB.end()) {
            removed.insert(path);
        } else if (contentA != it->second) {
            modified.insert(path);
        }
    }

    for (const auto& [path, _] : filesB) {
        if (!filesA.count(path)) {
            added.insert(path);
        }
    }

    for (const auto& dir : dirsA) {
        if (!dirsB.count(dir)) {
            removedDirs.insert(dir);
        }
    }

    for (const auto& dir : dirsB) {
        if (!dirsA.count(dir)) {
            addedDirs.insert(dir);
        }
    }

    const bool hasFileChanges = !added.empty() || !removed.empty() || !modified.empty();
    const bool hasDirChanges = !addedDirs.empty() || !removedDirs.empty();

    if (!hasFileChanges && !hasDirChanges) {
        return "No changes between snapshots.";
    }

    std::ostringstream oss;
    oss << "TreeDiff results:\n";
    for (const auto& path : added) {
        oss << "+ " << path << '\n';
    }
    for (const auto& path : removed) {
        oss << "- " << path << '\n';
    }
    for (const auto& path : modified) {
        oss << "~ " << path << '\n';
    }

    for (const auto& dir : addedDirs) {
        oss << "+ " << dir << " (dir)" << '\n';
    }
    for (const auto& dir : removedDirs) {
        oss << "- " << dir << " (dir)" << '\n';
    }

    return oss.str();
}

static std::vector<std::string> splitLines(const std::string& str) {
    std::vector<std::string> lines;
    std::istringstream stream(str);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

static std::vector<std::string> splitWords(const std::string& str) {
    std::vector<std::string> words;
    std::istringstream stream(str);
    std::string word;
    while (stream >> word) {
        words.push_back(word);
    }
    return words;
}

std::string LineDiffStrategy::diff(const std::shared_ptr<IFileSystemItem>& a,
                                   const std::shared_ptr<IFileSystemItem>& b) {
    auto aLines = splitLines(a->getContent());
    auto bLines = splitLines(b->getContent());

    std::ostringstream result;
    result << "LineDiff between " << a->getName() << " and " << b->getName() << ":\n";

    size_t maxLines = std::max(aLines.size(), bLines.size());

    for (size_t i = 0; i < maxLines; ++i) {
        const std::string& lineA = (i < aLines.size()) ? aLines[i] : "";
        const std::string& lineB = (i < bLines.size()) ? bLines[i] : "";

        if (lineA != lineB) {
            result << "- " << (lineA.empty() ? "[empty]" : lineA) << "\n";
            result << "+ " << (lineB.empty() ? "[empty]" : lineB) << "\n";
        }
    }

    return result.str();
}

std::string WordDiffStrategy::diff(const std::shared_ptr<IFileSystemItem>& a,
                                   const std::shared_ptr<IFileSystemItem>& b) {
    auto aWords = splitWords(a->getContent());
    auto bWords = splitWords(b->getContent());

    std::ostringstream result;
    result << "WordDiff between " << a->getName() << " and " << b->getName() << ":\n";

    size_t maxWords = std::max(aWords.size(), bWords.size());

    for (size_t i = 0; i < maxWords; ++i) {
        const std::string& wordA = (i < aWords.size()) ? aWords[i] : "";
        const std::string& wordB = (i < bWords.size()) ? bWords[i] : "";

        if (wordA != wordB) {
            result << "- " << (wordA.empty() ? "[empty]" : wordA) << "\n";
            result << "+ " << (wordB.empty() ? "[empty]" : wordB) << "\n";
        }
    }

    return result.str();
}
