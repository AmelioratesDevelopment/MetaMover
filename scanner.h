#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include "basicfilehandler.h"
#include "threadpool.h"

class Scanner : public QObject {
    Q_OBJECT

public:
    explicit Scanner(size_t threadPoolSize = std::thread::hardware_concurrency() / 2, QObject* parent = nullptr);
    ~Scanner();

public slots:
    void scan(const std::string& directoryPath, bool includeSubdirectories);

signals:
    void filesFoundUpdated(int filesFound);

private:
    void processFutures(std::vector<std::future<std::vector<std::unique_ptr<BasicFileHandler>>>>& futures, std::vector<std::unique_ptr<BasicFileHandler>>& handlers);
    std::vector<std::unique_ptr<BasicFileHandler>> scanDirectory(const std::string& directoryPath, bool includeSubdirectories);
    std::vector<std::unique_ptr<BasicFileHandler>> scanDirectorySingleThread(const std::string& directoryPath, bool includeSubdirectories);
    void emitFilesFound(int count);
    std::atomic<int> filesFound{0};
    std::vector<std::unique_ptr<BasicFileHandler>> basicFileHandlers;
    std::mutex mutex;
    ThreadPool threadPool;
};

#endif // SCANNER_H
