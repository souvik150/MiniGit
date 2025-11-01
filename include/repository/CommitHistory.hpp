//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_COMMITHISTORY_HPP
#define MINIGIT_COMMITHISTORY_HPP

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "Commit.hpp"


class CommitHistory {
private:
    std::vector<std::shared_ptr<Commit>> commits;

public:
    void addCommit(std::shared_ptr<Commit> commit);

    std::shared_ptr<Commit> getCommit(const std::string& id);

    std::shared_ptr<Commit> getLastCommit();

    void listCommits() const;

    void revertTo(const std::string& commitId);

    const std::vector<std::shared_ptr<Commit>>& getAllCommits() const {
        return commits;
    }
};

#endif //MINIGIT_COMMITHISTORY_HPP