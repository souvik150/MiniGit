//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_DIFFSTRATEGY_HPP
#define MINIGIT_DIFFSTRATEGY_HPP

#include <string>
#include "IFileSystemItem.hpp"

class IDiffStrategy {
public:
    virtual ~IDiffStrategy() = default;
    virtual std::string diff(const std::shared_ptr<IFileSystemItem>& a, const std::shared_ptr<IFileSystemItem>& b) = 0;
};

class TreeDiffStrategy : public IDiffStrategy {
public:
    std::string diff(const std::shared_ptr<IFileSystemItem>& a, const std::shared_ptr<IFileSystemItem>& b) override;
};

class LineDiffStrategy : public IDiffStrategy {
public:
    std::string diff(const std::shared_ptr<IFileSystemItem>& a, const std::shared_ptr<IFileSystemItem>& b) override;
};

class WordDiffStrategy : public IDiffStrategy {
public:
    std::string diff(const std::shared_ptr<IFileSystemItem>& a, const std::shared_ptr<IFileSystemItem>& b) override;
};


#endif //MINIGIT_DIFFSTRATEGY_HPP
