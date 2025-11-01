//
// Created by souvik on 11/1/25.
//
#include "Repository.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "CommitBuilder.hpp"
#include "DiffStrategyFactory.hpp"

Repository::Repository(std::shared_ptr<IFileSystemItem> root,
                       std::shared_ptr<IDiffStrategy> diffStrategy)
    : root(std::move(root)), diffStrategy(std::move(diffStrategy)), history() {
    if (!this->root) {
        throw std::invalid_argument("Repository requires a root item");
    }
    if (!this->diffStrategy) {
        throw std::invalid_argument("Repository requires a diff strategy");
    }
}

static std::string generateCommitId() {
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return "commit_" + std::to_string(millis);
}

static std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &now_time);
#else
    localtime_r(&now_time, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void Repository::commit(const std::string& message) {
    if (!root) {
        throw std::runtime_error("Repository root is empty");
    }
    if (!diffStrategy) {
        throw std::runtime_error("Diff strategy not set");
    }

    std::shared_ptr<IFileSystemItem> snapshot = std::shared_ptr<IFileSystemItem>(root->clone());

    std::string id = generateCommitId();
    std::string timestamp = getCurrentTimestamp();

    CommitBuilder builder;
    Commit newCommit = builder.setId(id)
                              .setMessage(message)
                              .setTimestamp(timestamp)
                              .setSnapshot(snapshot)
                              .build();

    history.addCommit(std::make_shared<Commit>(newCommit));
}

std::string Repository::diff(const std::shared_ptr<Commit>& commitA, const std::shared_ptr<Commit>& commitB) {
    if (!diffStrategy) {
        throw std::runtime_error("Diff strategy not set");
    }
    if (!commitA || !commitB) {
        throw std::invalid_argument("Invalid commits provided for diff");
    }

    return diffStrategy->diff(commitA->getSnapshot(), commitB->getSnapshot());
}

void Repository::restore(const std::string& commitId) {
    auto commit = history.getCommit(commitId);
    if (!commit) {
        throw std::runtime_error("Commit not found: " + commitId);
    }

    root = std::shared_ptr<IFileSystemItem>(commit->getSnapshot()->clone());
}

void Repository::setDiffStrategy(DiffStrategyType strategyName) {
    diffStrategy = std::shared_ptr<IDiffStrategy>(DiffStrategyFactory::getStrategy(strategyName));
}

std::shared_ptr<IFileSystemItem> Repository::getCurrentState() const {
    return root;
}

CommitHistory& Repository::getHistory() {
    return history;
}
