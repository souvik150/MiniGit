//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_REPOSITORY_HPP
#define MINIGIT_REPOSITORY_HPP
#include <memory>

#include "CommitHistory.hpp"
#include "DiffStrategy.hpp"
#include "DiffStrategyType.hpp"

class IFileSystemItem;

class Repository {
private:
    std::shared_ptr<IFileSystemItem> root;
    std::shared_ptr<IDiffStrategy> diffStrategy;
    CommitHistory history;

public:
    Repository(std::shared_ptr<IFileSystemItem> root,
                   std::shared_ptr<IDiffStrategy> diffStrategy);
    Repository() = delete;

    void commit(const std::string& message);

    std::string diff(const std::shared_ptr<Commit>& commitA, const std::shared_ptr<Commit>& commitB);

    void restore(const std::string& commitId);

    void setDiffStrategy(DiffStrategyType strategyName);

    [[nodiscard]] std::shared_ptr<IFileSystemItem> getCurrentState() const;

    CommitHistory& getHistory();
};

#endif //MINIGIT_REPOSITORY_HPP