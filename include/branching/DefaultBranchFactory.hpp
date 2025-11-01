//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_DEFAULTBRANCHFACTORY_HPP
#define MINIGIT_DEFAULTBRANCHFACTORY_HPP

#include <stdexcept>
#include "IBranchFactory.hpp"
#include "Branch.hpp"
#include "Folder.hpp"
#include "Repository.hpp"
#include "DiffStrategyFactory.hpp"

class DefaultBranchFactory : public IBranchFactory {
public:
    std::shared_ptr<Branch> createMainBranch(const std::string& name) override {
        auto emptyRoot = std::make_shared<Folder>("root");
        auto repo = std::make_shared<Repository>(
            emptyRoot,
            DiffStrategyFactory::getStrategy(DiffStrategyType::Line));
        return std::make_shared<Branch>(name, repo);
    }

    std::shared_ptr<Branch> createFromExisting(
        const std::string& name,
        const std::shared_ptr<Branch>& base) override {

        auto baseRepo = base->getRepository();
        auto baseState = baseRepo->getCurrentState();
        if (!baseState) {
            throw std::runtime_error("Base branch has no repository state to clone");
        }

        auto clonedRootUnique = baseState->clone();
        auto clonedRoot = std::shared_ptr<IFileSystemItem>(std::move(clonedRootUnique));
        auto newRepo = std::make_shared<Repository>(
            clonedRoot,
            DiffStrategyFactory::getStrategy(DiffStrategyType::Line));
        return std::make_shared<Branch>(name, newRepo);
    }
};

#endif //MINIGIT_DEFAULTBRANCHFACTORY_HPP
