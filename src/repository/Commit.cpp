//
// Created by souvik on 11/2/25.
//1
#include "Commit.hpp"

const std::string& Commit::getId() const {
    return id;
}

const std::string& Commit::getMessage() const {
    return message;
}

const std::string& Commit::getTimestamp() const {
    return timestamp;
}

std::shared_ptr<IFileSystemItem> Commit::getSnapshot() const {
    return snapshot;
}
