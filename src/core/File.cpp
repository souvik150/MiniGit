//
// Created by souvik on 11/1/25.
//
#include <iostream>
#include <utility>
#include "File.hpp"
#include <iostream>

File::File(std::string name, const std::string& content)
    : name(std::move(name)), content(content) {}

std::string File::getName() const {
    return name;
}

int File::getSize() const {
    return static_cast<int>(content.size());
}

void File::display(int indent) const {
    std::cout << std::string(indent, ' ') << name << "\n";
}

std::unique_ptr<IFileSystemItem> File::clone() const {
    return std::make_unique<File>(name, content);
}

std::string File::getContent() const {
    return content;
}