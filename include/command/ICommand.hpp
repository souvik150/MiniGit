//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_ICOMMAND_HPP
#define MINIGIT_ICOMMAND_HPP

#include <vector>
#include <string>
#include <iostream>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(const std::vector<std::string>& args) = 0;
};

class CommitCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

class DiffCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

class LogCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

class RevertCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

class BranchCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

class SwitchCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

class AddCommand : public ICommand {
public:
    void execute(const std::vector<std::string>& args) override;
};

#endif //MINIGIT_ICOMMAND_HPP