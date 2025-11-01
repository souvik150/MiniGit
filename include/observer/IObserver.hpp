//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_IOBSERVER_HPP
#define MINIGIT_IOBSERVER_HPP

#include <string>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(const std::string& eventName,
                          const std::string& payload) = 0;
};

#endif // MINIGIT_IOBSERVER_HPP

