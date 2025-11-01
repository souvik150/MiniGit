//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_COMMITBUILDER_HPP
#define MINIGIT_COMMITBUILDER_HPP

#include <string>
#include <memory>
#include <stdexcept>
#include "Commit.hpp"

class CommitBuilder {
private:
    Commit commit;

public:
    CommitBuilder& setId(const std::string& id) {
        commit.id = id;
        return *this;
    }

    CommitBuilder& setMessage(const std::string& message) {
        commit.message = message;
        return *this;
    }

    CommitBuilder& setTimestamp(const std::string& timestamp) {
        commit.timestamp = timestamp;
        return *this;
    }

    CommitBuilder& setSnapshot(std::shared_ptr<IFileSystemItem> snapshot) {
        commit.snapshot = std::move(snapshot);
        return *this;
    }

    Commit build() {
        if (commit.id.empty()) {
            throw std::runtime_error("Commit ID cannot be empty");
        }
        if (commit.message.empty()) {
            throw std::runtime_error("Commit message cannot be empty");
        }

        if (!commit.snapshot) {
            throw std::runtime_error("Commit snapshot cannot be null");
        }

        return commit;
    }
};

#endif //MINIGIT_COMMITBUILDER_HPP