//
// Created by souvik on 11/1/25.
//

#include "Folder.hpp"
#include <algorithm>
#include <iostream>

Folder::Folder(std::string name)
    : name(std::move(name)) {}

void Folder::addItem(std::unique_ptr<IFileSystemItem> item) {
    items.push_back(std::move(item));
}

void Folder::removeItem(const std::string& itemName) {
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const std::unique_ptr<IFileSystemItem>& item) {
            return item->getName() == itemName;
        }), items.end());
}

std::vector<const IFileSystemItem*> Folder::getItems() const {
    std::vector<const IFileSystemItem*> rawItems;
    for (const auto& item : items) {
        rawItems.push_back(item.get());
    }
    return rawItems;
}

std::string Folder::getName() const {
    return name;
}

int Folder::getSize() const {
    int total = 0;
    for (const auto& item : items) {
        total += item->getSize();
    }
    return total;
}

void Folder::display(int indent) const {
    std::cout << std::string(indent, ' ') << "+ " << name << "\n";
    for (const auto& item : items) {
        item->display(indent + 4);
    }
}

std::unique_ptr<IFileSystemItem> Folder::clone() const {
    auto folderClone = std::make_unique<Folder>(name);
    for (const auto& item : items) {
        folderClone->addItem(item->clone());
    }
    return folderClone;
}

std::string Folder::getContent() const {
    std::string combinedContent;
    for (const auto& item : items) {
        combinedContent += item->getContent();
    }
    return combinedContent;
}
