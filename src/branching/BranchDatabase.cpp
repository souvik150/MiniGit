//
// Created by souvik on 11/1/25.
//
#include "BranchDatabase.hpp"
#include <stdexcept>

#include "Folder.hpp"

BranchDatabase::BranchDatabase(std::shared_ptr<IBranchFactory> factory)
    : factory(std::move(factory)) {}

void BranchDatabase::initializeMain() {
    if (!branches.empty()) {
        throw std::runtime_error("Repository already initialized");
    }

    auto mainBranch = factory->createMainBranch("main");
    branches["main"] = mainBranch;
    activeBranch = mainBranch;
}

void BranchDatabase::createBranch(const std::string& name) {
    if (branches.empty()) {
        throw std::runtime_error("Repository not initialized. Run 'init' first.");
    }

    if (branches.find(name) != branches.end()) {
        throw std::runtime_error("Branch already exists: " + name);
    }

    auto baseBranch = getActiveBranch();
    auto newBranch = factory->createFromExisting(name, baseBranch);
    branches[name] = newBranch;
}

void BranchDatabase::switchBranch(const std::string& name) {
    auto it = branches.find(name);
    if (it == branches.end()) {
        throw std::runtime_error("Branch not found: " + name);
    }
    activeBranch = it->second;
}

std::shared_ptr<Branch> BranchDatabase::getActiveBranch() const {
    if (!activeBranch) {
        throw std::runtime_error("No active branch set");
    }
    return activeBranch;
}

void BranchDatabase::listBranches() const {
    std::cout << "Branches:\n";
    for (const auto& [name, branch] : branches) {
        if (activeBranch && name == activeBranch->getName()) {
            std::cout << "* " << name << " (active)\n";
        } else {
            std::cout << "  " << name << "\n";
        }
    }
}
