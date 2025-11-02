//
// Created by souvik on 11/1/25.
//
#include "GitManager.hpp"
#include "ICommand.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "DefaultBranchFactory.hpp"
#include "File.hpp"
#include "Folder.hpp"

std::unique_ptr<GitManager> GitManager::instance = nullptr;

bool GitManager::isInitialized() {
    return static_cast<bool>(instance);
}

GitManager::GitManager(std::unique_ptr<BranchDatabase> branchDB,
                       std::unique_ptr<NotificationService> notifier)
    : branchDBStorage(std::move(branchDB)),
      notifierStorage(std::move(notifier)),
      branchDatabase(*branchDBStorage),
      notificationService(*notifierStorage) {}

void GitManager::init(std::unique_ptr<BranchDatabase> branchDB,
                      std::unique_ptr<NotificationService> notifier) {
    if (instance) {
        throw std::runtime_error("GitManager already initialized");
    }
    if (!branchDB) {
        throw std::invalid_argument("Branch database cannot be null");
    }
    if (!notifier) {
        throw std::invalid_argument("Notification service cannot be null");
    }

    branchDB->initializeMain();
    instance.reset(new GitManager(std::move(branchDB), std::move(notifier)));
}

GitManager& GitManager::getInstance() {
    if (!instance)
        throw std::runtime_error("GitManager not initialized. Call GitManager::init() first.");
    return *instance;
}


void GitManager::addFile(const std::string& name, const std::string& content) {
    auto& root = workingRoot();
    root.addItem(std::make_unique<File>(name, content));
    notifier().notify("file_added", name);
}

void GitManager::addFolder(const std::string& name) {
    auto& root = workingRoot();
    root.addItem(std::make_unique<Folder>(name));
    notifier().notify("folder_added", name);
}

void GitManager::commit(const std::string& message) {
    auto& repo = repository();
    repo.commit(message);

    auto lastCommit = repo.getHistory().getLastCommit();
    assert(lastCommit);
    notifier().notify("commit",
                      lastCommit->getId() + " " + lastCommit->getMessage());
}

std::string GitManager::diffLast() const {
    auto& repo = repository();
    auto& history = repo.getHistory();

    auto commits = history.getAllCommits();

    if (commits.size() < 2) {
        return "Not enough commits to diff.";
    }

    const auto& lastCommit = commits.back();
    const auto& secondLastCommit = commits[commits.size() - 2];

    return repo.diff(secondLastCommit, lastCommit);
}

void GitManager::revert(const std::string& commitId) const {
    auto& repo = repository();
    repo.restore(commitId);
    notifier().notify("reverted", commitId);
}

void GitManager::createBranch(const std::string& name) const {
    database().createBranch(name);
    notifier().notify("branch_change", "created " + name);
}

void GitManager::switchBranch(const std::string& name) const {
    database().switchBranch(name);
    notifier().notify("branch_change", "switched " + name);
}

void GitManager::listCommits() const {
    repository().getHistory().listCommits();
}

void GitManager::listBranches() const {
    database().listBranches();
}

void GitManager::logCommits() const {
    repository().getHistory().listCommits();
}

BranchDatabase& GitManager::database() const {
    assert(branchDBStorage);
    return branchDatabase;
}

NotificationService& GitManager::notifier() const {
    assert(notifierStorage);
    return notificationService;
}

Repository& GitManager::repository() const {
    auto activeBranch = database().getActiveBranch();
    auto repo = activeBranch->getRepository();
    if (!repo) {
        throw std::runtime_error("Active branch has no repository");
    }
    return *repo;
}

Folder& GitManager::workingRoot() const {
    auto root = repository().getCurrentState();
    auto folderPtr = std::dynamic_pointer_cast<Folder>(root);
    if (!folderPtr) {
        throw std::runtime_error("Repository root is not a folder");
    }
    return *folderPtr;
}
