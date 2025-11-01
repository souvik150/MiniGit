//
// Created by souvik on 11/1/25.
//
#include "Branch.hpp"

Branch::Branch(std::string name, std::shared_ptr<Repository> repo)
    : name(std::move(name)), repository(std::move(repo)) {
}

std::string Branch::getName() const {
    return name;
}

std::shared_ptr<Repository> Branch::getRepository() const {
    return repository;
}