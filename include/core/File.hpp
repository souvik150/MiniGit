//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_FILE_HPP
#define MINIGIT_FILE_HPP

#include "IFileSystemItem.hpp"
#include <string>
#include <memory>

class File : public IFileSystemItem {
    std::string name;
    std::string content;

public:
    File(std::string name, const std::string& content);

    std::string getName() const override;
    int getSize() const override;
    void display(int indent) const override;
    std::unique_ptr<IFileSystemItem> clone() const;
    std::string getContent() const override;
};

#endif //MINIGIT_FILE_HPP