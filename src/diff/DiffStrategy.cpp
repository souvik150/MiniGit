//
// Created by souvik on 11/1/25.
//
#include "DiffStrategy.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

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
