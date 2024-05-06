#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include "basicfilehandler.h"
#include "filehandlerfactory.h"

class Scanner : public QObject {
    Q_OBJECT

public:
    explicit Scanner(QObject* parent = nullptr);
    int const getTotalFilesFound();
    int const getTotalPhotoFilesFound();
    int const getPhotoFilesFoundContainingEXIFData();
    int const getPhotoFilesFoundContainingValidCreationDate();
    int const getPhotoFilesFoundContainingEXIFWODate();
    ~Scanner();

public slots:
    void scan(const std::string& directoryPath, bool includeSubdirectories);
    void resetScanner();

signals:
    void filesFoundUpdated(int filesFound);
    void scanCompleted();

private:
    void scanDirectory(const std::string& directoryPath, bool includeSubdirectories);
    std::string escapeBackslashes(const std::string& input);
    void emitPeriodically();
    std::atomic<int> filesFound{0};
    std::atomic<int> photoFilesFoundContainingEXIFData{0};
    std::atomic<int> photoFilesFoundContainingValidCreationDate{0};
    std::atomic<int> photoFilesFoundContainingEXIFWODate{0};
    std::vector<std::unique_ptr<BasicFileHandler>> basicFileHandlers;
    std::vector<std::unique_ptr<PhotoFileHandler>> photoFileHandlers;
    std::vector<std::unique_ptr<VideoFileHandler>> videoFileHandlers;
    std::mutex mutex;
    std::atomic<bool> stopTimer;
    FileFactory fileFactory;
};

#endif // SCANNER_H
