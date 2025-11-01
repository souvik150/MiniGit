//
// Created by souvik on 11/1/25.
//
#include "HistoryFileObserver.hpp"

#include <fstream>
#include <stdexcept>

HistoryFileObserver::HistoryFileObserver(std::string historyPath)
    : filePath(std::move(historyPath)) {}

void HistoryFileObserver::onNotify(const std::string& eventName,
                                   const std::string& payload) {
    const auto prefix = mapEventPrefix(eventName);
    appendLine(prefix + " " + payload);
}

std::string HistoryFileObserver::mapEventPrefix(const std::string& eventName) const {
    if (eventName == "commit") {
        return "[commit_hook]";
    }
    if (eventName == "branch_change") {
        return "[branch_change_hook]";
    }
    if (eventName == "file_added") {
        return "[file_hook]";
    }
    if (eventName == "folder_added") {
        return "[folder_hook]";
    }
    if (eventName == "reverted") {
        return "[revert_hook]";
    }
    return "[hook]";
}

void HistoryFileObserver::appendLine(const std::string& line) {
    std::lock_guard<std::mutex> lock(writeMutex);
    std::ofstream out(filePath, std::ios::app);
    if (!out) {
        throw std::runtime_error("Unable to open history log: " + filePath.string());
    }
    out << line << '\n';
}
