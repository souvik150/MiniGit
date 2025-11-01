//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_IBRANCHFACTORY_HPP
#define MINIGIT_IBRANCHFACTORY_HPP

#include <memory>
#include <string>

class Branch;

class IBranchFactory {
public:
    virtual ~IBranchFactory() = default;
    virtual std::shared_ptr<Branch> createMainBranch(const std::string& name) = 0;
    virtual std::shared_ptr<Branch> createFromExisting(const std::string& name,
                                                       const std::shared_ptr<Branch>& base) = 0;
};

#endif //MINIGIT_IBRANCHFACTORY_HPP