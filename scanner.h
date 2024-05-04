#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include "basicfilehandler.h"

class Scanner : public QObject {
    Q_OBJECT

public:
    explicit Scanner(QObject* parent = nullptr);
    ~Scanner();

public slots:
    void scan(const std::string& directoryPath, bool includeSubdirectories);

signals:
    void filesFoundUpdated(int filesFound);

private:
    std::vector<std::unique_ptr<BasicFileHandler>> scanDirectorySingleThread(const std::string& directoryPath, bool includeSubdirectories);
    void emitFilesFound(int count);
    void emitPeriodically();
    std::atomic<int> filesFound{0};
    std::vector<std::unique_ptr<BasicFileHandler>> basicFileHandlers;
    std::vector<std::unique_ptr<BasicFileHandler>> photoFileHandlers;
    std::vector<std::unique_ptr<BasicFileHandler>> videoFileHandlers;
    std::mutex mutex;
    std::atomic<bool> stopTimer;
};

#endif // SCANNER_H
