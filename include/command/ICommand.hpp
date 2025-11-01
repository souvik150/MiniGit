//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_ICOMMAND_HPP
#define MINIGIT_ICOMMAND_HPP

#include <vector>
#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(const std::vector<std::string>& args) = 0;
};

class GitManager;

class CommitCommand : public ICommand {
public:
    explicit CommitCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

class DiffCommand : public ICommand {
public:
    explicit DiffCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

class LogCommand : public ICommand {
public:
    explicit LogCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

class RevertCommand : public ICommand {
public:
    explicit RevertCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

class BranchCommand : public ICommand {
public:
    explicit BranchCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

class SwitchCommand : public ICommand {
public:
    explicit SwitchCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

class AddCommand : public ICommand {
public:
    explicit AddCommand(GitManager& manager);
    void execute(const std::vector<std::string>& args) override;

private:
    GitManager& gitManager;
};

#endif //MINIGIT_ICOMMAND_HPP
