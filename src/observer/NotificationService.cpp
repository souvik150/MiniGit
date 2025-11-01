//
// Created by souvik on 11/1/25.
//
#include "NotificationService.hpp"

void NotificationService::addObserver(std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }
    std::lock_guard<std::mutex> lock(mutex);
    observers.push_back(std::move(observer));
}

void NotificationService::notify(const std::string& eventName,
                                 const std::string& payload) const {
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto& observer : observers) {
        if (observer) {
            observer->onNotify(eventName, payload);
        }
    }
}
