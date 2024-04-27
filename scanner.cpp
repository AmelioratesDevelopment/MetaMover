#include "scanner.h"
#include <filesystem>

Scanner::Scanner(size_t threadPoolSize, QObject* parent) : QObject(parent), threadPool(threadPoolSize) {}

Scanner::~Scanner() {}

void Scanner::scan(const std::string& directoryPath, bool includeSubdirectories) {
    std::lock_guard<std::mutex> lock(mutex);
    basicFileHandlers.clear();
    filesFound = 0;

    auto handlers = scanDirectorySingleThread(directoryPath, includeSubdirectories);
    basicFileHandlers = std::move(handlers);

    emitFilesFound(filesFound.load());
}

std::vector<std::unique_ptr<BasicFileHandler>> Scanner::scanDirectorySingleThread(const std::string& directoryPath, bool includeSubdirectories) {
    std::vector<std::unique_ptr<BasicFileHandler>> handlers;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_directory() && includeSubdirectories) {
            // Recursively call this function instead of enqueuing tasks
            auto subdirHandlers = scanDirectorySingleThread(entry.path().string(), true);
            handlers.insert(handlers.end(), std::make_move_iterator(subdirHandlers.begin()), std::make_move_iterator(subdirHandlers.end()));
        } else if (!entry.is_directory()) {
            handlers.push_back(std::make_unique<BasicFileHandler>(entry.path().string()));
            emitFilesFound(filesFound.fetch_add(1) + 1);
        }
    }

    return handlers;
}

std::vector<std::unique_ptr<BasicFileHandler>> Scanner::scanDirectory(const std::string& directoryPath, bool includeSubdirectories) {
    std::vector<std::future<std::vector<std::unique_ptr<BasicFileHandler>>>> futures;
    std::vector<std::unique_ptr<BasicFileHandler>> handlers;
    const size_t maxActiveFutures = 10;  // Maximum concurrent directory scans

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_directory() && includeSubdirectories) {
            if (futures.size() >= maxActiveFutures) {
                // Wait for one future to complete before continuing
                processFutures(futures, handlers);
            }
            futures.push_back(threadPool.enqueue([this, path = entry.path().string()] {
                return scanDirectory(path, true);
            }));
        } else if (!entry.is_directory()) {
            handlers.push_back(std::make_unique<BasicFileHandler>(entry.path().string()));
            emitFilesFound(filesFound.fetch_add(1) + 1);
        }
    }

    // Process remaining futures
    processFutures(futures, handlers);

    return handlers;
}

void Scanner::processFutures(std::vector<std::future<std::vector<std::unique_ptr<BasicFileHandler>>>>& futures, std::vector<std::unique_ptr<BasicFileHandler>>& handlers) {
    // Wait for the first future to complete
    auto it = std::find_if(futures.begin(), futures.end(), [](auto& fut) {
        return fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    });
    if (it != futures.end()) {
        auto result = it->get();
        handlers.insert(handlers.end(), std::make_move_iterator(result.begin()), std::make_move_iterator(result.end()));
        futures.erase(it);
    }
}

void Scanner::emitFilesFound(int count) {
    emit filesFoundUpdated(count);
}
