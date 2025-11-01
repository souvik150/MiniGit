//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_NOTIFICATIONSERVICE_HPP
#define MINIGIT_NOTIFICATIONSERVICE_HPP

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "IObserver.hpp"

class NotificationService {
public:
    void addObserver(std::shared_ptr<IObserver> observer);
    void notify(const std::string& eventName, const std::string& payload) const;

private:
    mutable std::mutex mutex;
    std::vector<std::shared_ptr<IObserver>> observers;
};

#endif // MINIGIT_NOTIFICATIONSERVICE_HPP
