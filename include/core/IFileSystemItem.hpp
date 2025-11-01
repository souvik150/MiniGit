//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_IFILESYSTEM_H
#define MINIGIT_IFILESYSTEM_H
#include <string>
#include <memory>

class IFileSystemItem {
public:
    virtual ~IFileSystemItem() = default;

    virtual std::string getName() const = 0;
    virtual int getSize() const = 0;
    virtual void display(int indent) const = 0;
    virtual std::unique_ptr<IFileSystemItem> clone() const = 0;
    virtual std::string getContent() const = 0;
};
#endif //MINIGIT_IFILESYSTEM_H