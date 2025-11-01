//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_CLI_HPP
#define MINIGIT_CLI_HPP
#include <string>
#include <vector>
#include <memory>

class GitManager;

class CLI {
public:
    CLI();
    void start();
    void parseCommand(const std::string& cmdLine);

private:
    GitManager& gitManager;
    static GitManager& bootstrapGitManager();
    std::string normalizeCommitId(const std::string& input) const;
    std::vector<std::string> split(const std::string& str);
};

#endif //MINIGIT_CLI_HPP
