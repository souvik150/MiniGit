//
// Created by souvik on 11/1/25.
//
#include "CommitHistory.hpp"
#include "Commit.hpp"

#include <algorithm>
#include <stdexcept>
#include <iostream>

void CommitHistory::addCommit(std::shared_ptr<Commit> commit) {
    commits.push_back(std::move(commit));
}

std::shared_ptr<Commit> CommitHistory::getCommit(const std::string& id) {
    auto it = std::find_if(commits.begin(), commits.end(),
                           [&id](const std::shared_ptr<Commit>& c) { return c->getId() == id; });
    if (it != commits.end()) {
        return *it;
    }
    return nullptr;
}

std::shared_ptr<Commit> CommitHistory::getLastCommit() {
    if (commits.empty()) {
        return nullptr;
    }
    return commits.back();
}

void CommitHistory::listCommits() const {
    for (const auto& commit : commits) {
        std::cout << "Commit ID: " << commit->getId()
                  << ", Message: " << commit->getMessage()
                  << ", Timestamp: " << commit->getTimestamp() << std::endl;
    }
}

void CommitHistory::revertTo(const std::string& commitId) {
    auto it = std::find_if(commits.begin(), commits.end(),
                           [&commitId](const std::shared_ptr<Commit>& c) { return c->getId() == commitId; });

    if (it == commits.end()) {
        throw std::runtime_error("Commit ID not found: " + commitId);
    }

    // Erase commits after the found commit (keep commitId and all before it)
    commits.erase(std::next(it), commits.end());
}

