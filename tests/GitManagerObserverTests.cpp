//
// Observer logging tests for MiniGit
//

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "BranchDatabase.hpp"
#include "DefaultBranchFactory.hpp"
#include "GitManager.hpp"
#include "HistoryFileObserver.hpp"
#include "IObserver.hpp"
#include "NotificationService.hpp"
#include "DiffStrategy.hpp"
#include "Folder.hpp"
#include "File.hpp"

namespace {
class CapturingObserver : public IObserver {
public:
    std::vector<std::pair<std::string, std::string>> events;
    std::string lastCommitId;

    void onNotify(const std::string& eventName,
                  const std::string& payload) override {
        events.emplace_back(eventName, payload);
        if (eventName == "commit") {
            auto spacePos = payload.find(' ');
            lastCommitId = payload.substr(0, spacePos);
        }
    }
};

bool startsWith(const std::string& value, const std::string& prefix) {
    return value.rfind(prefix, 0) == 0;
}
} // namespace

int main() {
    using namespace std::string_literals;
    auto tempPath = std::filesystem::temp_directory_path() / "minigit_history_test.log";
    std::filesystem::remove(tempPath);

    auto branchFactory = std::make_shared<DefaultBranchFactory>();
    auto branchDB = std::make_unique<BranchDatabase>(branchFactory);
    auto notifier = std::make_unique<NotificationService>();
    auto capturingObserver = std::make_shared<CapturingObserver>();
    auto historyObserver = std::make_shared<HistoryFileObserver>(tempPath.string());

    notifier->addObserver(capturingObserver);
    notifier->addObserver(historyObserver);

    GitManager::init(std::move(branchDB), std::move(notifier));
    auto& gm = GitManager::getInstance();

    gm.addFile("foo.txt", "hello world");
    gm.addFolder("docs");
    gm.commit("initial commit");

    if (capturingObserver->lastCommitId.empty()) {
        std::cerr << "Commit observer did not capture commit id\n";
        return 1;
    }

    gm.revert(capturingObserver->lastCommitId);
    gm.createBranch("feature");
    gm.switchBranch("feature");

    const std::vector<std::string> expectedEvents = {
        "file_added",
        "folder_added",
        "commit",
        "reverted",
        "branch_change",
        "branch_change",
    };

    if (capturingObserver->events.size() != expectedEvents.size()) {
        std::cerr << "Expected " << expectedEvents.size()
                  << " events but observed " << capturingObserver->events.size() << "\n";
        return 1;
    }

    for (std::size_t i = 0; i < expectedEvents.size(); ++i) {
        if (capturingObserver->events[i].first != expectedEvents[i]) {
            std::cerr << "Event " << i << " expected '" << expectedEvents[i]
                      << "' but was '" << capturingObserver->events[i].first << "'\n";
            return 1;
        }
    }

    std::ifstream historyFile(tempPath);
    if (!historyFile) {
        std::cerr << "Unable to open history log at " << tempPath << "\n";
        return 1;
    }

    std::vector<std::string> historyLines;
    std::string line;
    while (std::getline(historyFile, line)) {
        historyLines.push_back(line);
    }
    historyFile.close();

    if (historyLines.size() != expectedEvents.size()) {
        std::cerr << "Expected " << expectedEvents.size()
                  << " history lines but found " << historyLines.size() << "\n";
        return 1;
    }

    if (!startsWith(historyLines[0], "[file_hook] foo.txt")) {
        std::cerr << "First history line unexpected: " << historyLines[0] << "\n";
        return 1;
    }
    if (!startsWith(historyLines[1], "[folder_hook] docs")) {
        std::cerr << "Second history line unexpected: " << historyLines[1] << "\n";
        return 1;
    }
    if (!startsWith(historyLines[2], "[commit_hook] " + capturingObserver->lastCommitId)) {
        std::cerr << "Commit history line unexpected: " << historyLines[2] << "\n";
        return 1;
    }
    if (!startsWith(historyLines[3], "[revert_hook] " + capturingObserver->lastCommitId)) {
        std::cerr << "Revert history line unexpected: " << historyLines[3] << "\n";
        return 1;
    }
    if (!startsWith(historyLines[4], "[branch_change_hook] created feature")) {
        std::cerr << "Branch create history line unexpected: " << historyLines[4] << "\n";
        return 1;
    }
    if (!startsWith(historyLines[5], "[branch_change_hook] switched feature")) {
        std::cerr << "Branch switch history line unexpected: " << historyLines[5] << "\n";
        return 1;
    }

    TreeDiffStrategy treeStrategy;
    auto base = std::make_shared<Folder>("root");
    base->addItem(std::make_unique<File>("foo.txt", "alpha"));

    auto updated = std::make_shared<Folder>("root");
    updated->addItem(std::make_unique<File>("foo.txt", "beta"));
    updated->addItem(std::make_unique<Folder>("docs"));

    auto diffResult = treeStrategy.diff(
        std::static_pointer_cast<IFileSystemItem>(base),
        std::static_pointer_cast<IFileSystemItem>(updated));

    if (diffResult.find("~ foo.txt") == std::string::npos) {
        std::cerr << "Diff output missing modified marker: " << diffResult << "\n";
        return 1;
    }
    if (diffResult.find("+ docs/") == std::string::npos) {
        std::cerr << "Diff output missing added directory: " << diffResult << "\n";
        return 1;
    }

    std::filesystem::remove(tempPath);
    return 0;
}
