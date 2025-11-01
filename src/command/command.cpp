#include "ICommand.hpp"

void CommitCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Commit message missing.\n";
        return;
    }
    std::cout << "Committing with message: ";
    for (const auto& word : args) std::cout << word << " ";
    std::cout << "\n";
    // TODO: Connect to GitManager::commit(...)
}

void DiffCommand::execute(const std::vector<std::string>&) {
    std::cout << "Diff command executed.\n";
    // TODO: Implement diff logic
}

void LogCommand::execute(const std::vector<std::string>&) {
    std::cout << "Log command executed.\n";
    // TODO: Implement log display
}

void RevertCommand::execute(const std::vector<std::string>&) {
    std::cout << "Revert command executed.\n";
    // TODO: Implement revert logic
}

void BranchCommand::execute(const std::vector<std::string>&) {
    std::cout << "Branch command executed.\n";
    // TODO: Implement branch creation or listing
}

void SwitchCommand::execute(const std::vector<std::string>&) {
    std::cout << "Switch command executed.\n";
    // TODO: Implement branch switching
}

void AddCommand::execute(const std::vector<std::string>&) {
    std::cout << "Add command executed.\n";
    // TODO: Implement add file/folder
}
