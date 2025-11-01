//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_CLI_HPP
#define MINIGIT_CLI_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ICommand.hpp"

class GitManager;

class CLI {
public:
    CLI();
    void start();
    void parseCommand(const std::string& cmdLine);

private:
    static GitManager& bootstrapGitManager();
    void registerCommands();
    std::unordered_map<std::string, std::unique_ptr<ICommand>> commandRegistry;
    std::vector<std::string> split(const std::string& str);
};

#endif //MINIGIT_CLI_HPP
