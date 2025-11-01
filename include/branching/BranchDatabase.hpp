//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_BRANCHDATABASE_HPP
#define MINIGIT_BRANCHDATABASE_HPP

#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "Branch.hpp"
#include "IBranchFactory.hpp"

class BranchDatabase {
private:
    std::map<std::string, std::shared_ptr<Branch>> branches;
    std::shared_ptr<Branch> activeBranch;
    std::shared_ptr<IBranchFactory> factory;

public:
    explicit BranchDatabase(std::shared_ptr<IBranchFactory> factory);
    void initializeMain();

    void createBranch(const std::string& name);
    void switchBranch(const std::string& name);
    std::shared_ptr<Branch> getActiveBranch() const;
    void listBranches() const;
};

#endif //MINIGIT_BRANCHDATABASE_HPP