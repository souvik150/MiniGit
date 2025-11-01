//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_HISTORYFILEOBSERVER_HPP
#define MINIGIT_HISTORYFILEOBSERVER_HPP

#include <filesystem>
#include <mutex>
#include <string>

#include "IObserver.hpp"

class HistoryFileObserver : public IObserver {
public:
    explicit HistoryFileObserver(std::string historyPath = ".history");
    void onNotify(const std::string& eventName,
                  const std::string& payload) override;

private:
    std::filesystem::path filePath;
    std::mutex writeMutex;
    std::string mapEventPrefix(const std::string& eventName) const;
    void appendLine(const std::string& line);
};

#endif // MINIGIT_HISTORYFILEOBSERVER_HPP

