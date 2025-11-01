//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_FOLDER_HPP
#define MINIGIT_FOLDER_HPP
#include "IFileSystemItem.hpp"
#include <string>
#include <vector>
#include <memory>

class Folder : public IFileSystemItem {
    std::string name;
    std::vector<std::unique_ptr<IFileSystemItem>> items;

public:
    explicit Folder(std::string name);

    void addItem(std::unique_ptr<IFileSystemItem> item);

    void removeItem(const std::string& itemName);

    std::vector<const IFileSystemItem*> getItems() const;

    std::string getName() const override;

    int getSize() const override;

    void display(int indent) const override;

    std::unique_ptr<IFileSystemItem> clone() const override;

    std::string getContent() const override;
};

#endif //MINIGIT_FOLDER_HPP