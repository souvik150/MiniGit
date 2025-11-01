//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_GITMANAGER_HPP
#define MINIGIT_GITMANAGER_HPP
#include <memory>
#include <string>
#include "BranchDatabase.hpp"
#include "NotificationService.hpp"

class Repository;
class Folder;

class GitManager {
private:
    std::unique_ptr<BranchDatabase> branchDBStorage;
    std::unique_ptr<NotificationService> notifierStorage;
    BranchDatabase& branchDatabase;
    NotificationService& notificationService;

    static std::unique_ptr<GitManager> instance;

    GitManager(std::unique_ptr<BranchDatabase> branchDB,
               std::unique_ptr<NotificationService> notifier);



    [[nodiscard]] BranchDatabase& database() const;
    [[nodiscard]] NotificationService& notifier() const;
    [[nodiscard]] Repository& repository() const;
    [[nodiscard]] Folder& workingRoot() const;

public:
    GitManager(const GitManager&) = delete;
    GitManager& operator=(const GitManager&) = delete;

    static void init(std::unique_ptr<BranchDatabase> branchDB,
                     std::unique_ptr<NotificationService> notifier);
    static GitManager& getInstance();

    void addFile(const std::string& name, const std::string& content);
    void addFolder(const std::string& name);
    void commit(const std::string& message);
    [[nodiscard]] std::string diffLast() const;
    void revert(const std::string& commitId) const;
    void createBranch(const std::string& name) const;
    void switchBranch(const std::string& name) const;
    void listCommits() const;
    void listBranches() const;
    void logCommits() const;
};
#endif //MINIGIT_GITMANAGER_HPP
