//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_COMMIT_HPP
#define MINIGIT_COMMIT_HPP

#include <string>
#include <memory>
#include "IFileSystemItem.hpp"

class CommitBuilder;

class Commit {
private:
    std::string id;
    std::string message;
    std::string timestamp;
    std::shared_ptr<IFileSystemItem> snapshot;

    Commit() = default;

    friend class CommitBuilder;

public:
    const std::string& getId() const { return id; }
    const std::string& getMessage() const { return message; }
    const std::string& getTimestamp() const { return timestamp; }
    std::shared_ptr<IFileSystemItem> getSnapshot() const { return snapshot; }
};

#endif //MINIGIT_COMMIT_HPP