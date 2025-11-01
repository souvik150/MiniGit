//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_BRANCH_HPP
#define MINIGIT_BRANCH_HPP

#include <string>
#include <memory>
#include "Repository.hpp"

class Branch {
private:
    std::string name;
    std::shared_ptr<Repository> repository;

public:
    Branch(std::string name, std::shared_ptr<Repository> repo);

    std::string getName() const;
    std::shared_ptr<Repository> getRepository() const;
};

#endif //MINIGIT_BRANCH_HPP