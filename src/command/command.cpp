#include "ICommand.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

#include "GitManager.hpp"

namespace {
std::string joinMessage(const std::vector<std::string>& args) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (i) {
            oss << ' ';
        }
        oss << args[i];
    }
    return oss.str();
}

std::string normalizeCommitId(const std::string& input) {
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
}

CommitCommand::CommitCommand(GitManager& manager) : gitManager(manager) {}

void CommitCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: commit <message>\n";
        return;
    }

    gitManager.commit(joinMessage(args));
    std::cout << "Commit created.\n";
}

DiffCommand::DiffCommand(GitManager& manager) : gitManager(manager) {}

void DiffCommand::execute(const std::vector<std::string>&) {
    std::cout << gitManager.diffLast() << "\n";
}

LogCommand::LogCommand(GitManager& manager) : gitManager(manager) {}

void LogCommand::execute(const std::vector<std::string>&) {
    gitManager.logCommits();
}

RevertCommand::RevertCommand(GitManager& manager) : gitManager(manager) {}

void RevertCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: revert <commit_id>\n";
        return;
    }

    auto commitId = normalizeCommitId(args.front());
    gitManager.revert(commitId);
    std::cout << "Reverted to commit: " << commitId << "\n";
}

BranchCommand::BranchCommand(GitManager& manager) : gitManager(manager) {}

void BranchCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        gitManager.listBranches();
        return;
    }

    gitManager.createBranch(args.front());
    std::cout << "Branch created: " << args.front() << "\n";
}

SwitchCommand::SwitchCommand(GitManager& manager) : gitManager(manager) {}

void SwitchCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: switch <branch_name>\n";
        return;
    }

    gitManager.switchBranch(args.front());
    std::cout << "Switched to branch: " << args.front() << "\n";
}

AddCommand::AddCommand(GitManager& manager) : gitManager(manager) {}

void AddCommand::execute(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: add <file|folder> <name>\n";
        return;
    }

    const auto& targetType = args[0];
    const auto& name = args[1];

    if (targetType == "file") {
        gitManager.addFile(name, "");
        std::cout << "File added: " << name << "\n";
    } else if (targetType == "folder") {
        gitManager.addFolder(name);
        std::cout << "Folder added: " << name << "\n";
    } else {
        std::cout << "Unknown add type. Use 'file' or 'folder'.\n";
    }
}
