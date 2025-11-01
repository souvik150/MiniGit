//
// Created by souvik on 11/1/25.
//
#include "CLI.hpp"
#include "GitManager.hpp"
#include "BranchDatabase.hpp"
#include "NotificationService.hpp"
#include "HistoryFileObserver.hpp"
#include "ICommand.hpp"
#include <iostream>
#include <memory>
#include <sstream>

#include "DefaultBranchFactory.hpp"

CLI::CLI() {
    try {
        (void)GitManager::getInstance();
    } catch (const std::exception&) {
        bootstrapGitManager();
    }
    registerCommands();
}

void CLI::start() {
    std::string line;
    std::cout << "MiniGit CLI started. Enter commands (type 'exit' to quit).\n";
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;

        parseCommand(line);
    }
}

void CLI::parseCommand(const std::string& cmdLine) {
    auto tokens = split(cmdLine);
    if (tokens.empty()) return;

    const std::string& cmd = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    auto it = commandRegistry.find(cmd);
    if (it == commandRegistry.end()) {
        std::cout << "Unknown command: " << cmd << "\n";
        return;
    }

    try {
        it->second->execute(args);
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << "\n";
    }
}

std::vector<std::string> CLI::split(const std::string& str) {
    std::stringstream ss(str);
    std::string word;
    std::vector<std::string> tokens;
    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

GitManager& CLI::bootstrapGitManager() {
    auto branchFactory = std::make_shared<DefaultBranchFactory>();
    auto branchDB = std::make_unique<BranchDatabase>(branchFactory);
    auto notifier = std::make_unique<NotificationService>();
    notifier->addObserver(std::make_shared<HistoryFileObserver>());

    GitManager::init(std::move(branchDB), std::move(notifier));
    return GitManager::getInstance();
}

void CLI::registerCommands() {
    auto& gitManager = GitManager::getInstance();
    commandRegistry.emplace("add", std::make_unique<AddCommand>(gitManager));
    commandRegistry.emplace("commit", std::make_unique<CommitCommand>(gitManager));
    commandRegistry.emplace("diff", std::make_unique<DiffCommand>(gitManager));
    commandRegistry.emplace("log", std::make_unique<LogCommand>(gitManager));
    commandRegistry.emplace("revert", std::make_unique<RevertCommand>(gitManager));
    commandRegistry.emplace("branch", std::make_unique<BranchCommand>(gitManager));
    commandRegistry.emplace("switch", std::make_unique<SwitchCommand>(gitManager));
}
