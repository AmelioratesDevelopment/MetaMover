#include "scanner.h"
#include <filesystem>
#include <QDebug>
#include <chrono>
#include <thread>
#include <atomic>

Scanner::Scanner(QObject* parent) : QObject(parent), stopTimer(false) {}

Scanner::~Scanner() {}

void Scanner::scan(const std::string& directoryPath, bool includeSubdirectories) {
    std::lock_guard<std::mutex> lock(mutex);
    basicFileHandlers.clear();
    filesFound = 0;
    stopTimer = false;

    // Start timer thread
    std::thread timerThread(&Scanner::emitPeriodically, this);

    auto handlers = scanDirectorySingleThread(directoryPath, includeSubdirectories);
    basicFileHandlers = std::move(handlers);

    // Stop timer thread and ensure final emit
    stopTimer = true;
    timerThread.join();
    emitFilesFound(filesFound.load());
}

std::vector<std::unique_ptr<BasicFileHandler>> Scanner::scanDirectorySingleThread(const std::string& directoryPath, bool includeSubdirectories) {
    std::vector<std::unique_ptr<BasicFileHandler>> handlers;
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_directory() && includeSubdirectories) {
            auto subdirHandlers = scanDirectorySingleThread(entry.path().string(), true);
            handlers.insert(handlers.end(), std::make_move_iterator(subdirHandlers.begin()), std::make_move_iterator(subdirHandlers.end()));
        } else if (!entry.is_directory()) {
            handlers.push_back(std::make_unique<BasicFileHandler>(entry.path().string()));
            filesFound++;
        }
    }
    return handlers;
}

void Scanner::emitFilesFound(int count) {
    emit filesFoundUpdated(count);
}

void Scanner::emitPeriodically() {
    while (!stopTimer) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        emitFilesFound(filesFound.load());
    }
}
