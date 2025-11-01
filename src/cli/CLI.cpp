//
// Created by souvik on 11/1/25.
//
#include "CLI.hpp"
#include "GitManager.hpp"
#include "BranchDatabase.hpp"
#include "NotificationService.hpp"
#include "HistoryFileObserver.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <memory>

#include "DefaultBranchFactory.hpp"

CLI::CLI() : gitManager(bootstrapGitManager()) {}

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

    try {
        if (cmd == "add") {
            if (args.size() < 2) {
                std::cout << "Usage: add <file|folder> <name>\n";
                return;
            }
            if (args[0] == "file") {
                gitManager.addFile(args[1], "");  // content empty for now
                std::cout << "File added: " << args[1] << "\n";
            } else if (args[0] == "folder") {
                gitManager.addFolder(args[1]);
                std::cout << "Folder added: " << args[1] << "\n";
            } else {
                std::cout << "Unknown add type. Use 'file' or 'folder'.\n";
            }
        }
        else if (cmd == "commit") {
            if (args.empty()) {
                std::cout << "Usage: commit <message>\n";
                return;
            }
            std::string message;
            for (const auto& word : args) message += word + " ";
            gitManager.commit(message);
            std::cout << "Commit created.\n";
        }
        else if (cmd == "diff") {
            std::cout << gitManager.diffLast() << "\n";
        }
        else if (cmd == "revert") {
            if (args.empty()) {
                std::cout << "Usage: revert <commit_id>\n";
                return;
            }
            auto commitId = normalizeCommitId(args[0]);
            gitManager.revert(commitId);
            std::cout << "Reverted to commit: " << commitId << "\n";
        }
        else if (cmd == "branch") {
            if (args.empty()) {
                gitManager.listBranches();
            } else {
                gitManager.createBranch(args[0]);
                std::cout << "Branch created: " << args[0] << "\n";
            }
        }
        else if (cmd == "switch") {
            if (args.empty()) {
                std::cout << "Usage: switch <branch_name>\n";
                return;
            }
            gitManager.switchBranch(args[0]);
            std::cout << "Switched to branch: " << args[0] << "\n";
        }
        else if (cmd == "log") {
            gitManager.logCommits();
        }
        else {
            std::cout << "Unknown command: " << cmd << "\n";
        }
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

std::string CLI::normalizeCommitId(const std::string& input) const {
    if (input.rfind("commit_", 0) == 0) {
        return input;
    }

    const bool allDigits = !input.empty() &&
        std::all_of(input.begin(), input.end(), [](unsigned char ch) { return std::isdigit(ch); });

    if (allDigits) {
        return "commit_" + input;
    }

    return input;
}
