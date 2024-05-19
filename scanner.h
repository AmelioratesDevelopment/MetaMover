#ifndef SCANNER_H
#define SCANNER_H

/***********************************************************************
 * File Name: Scanner.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the Scanner class, responsible for scanning
 *              directories and processing files to extract metadata such as
 *              EXIF data. This class manages the identification of different
 *              file types and collects relevant data to determine handling
 *              strategies for file transfers or copies.
 * License: MIT License
 ***********************************************************************/


#include <QObject>
#include <QTimer>
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
    bool checkScanResults(bool showMessage);
    bool cancelScan;
    std::string sanitizeFilePath(const std::string& path);
    std::vector<std::unique_ptr<PhotoFileHandler>>& getPhotoFileHandlers();
    std::vector<std::unique_ptr<PhotoFileHandler>>& getInvalidPhotoFileHandlers();
    int const getTotalFilesFound();
    int const getTotalPhotoFilesFound();
    int const getPhotoFilesFoundContainingEXIFData();
    int const getPhotoFilesFoundContainingValidCreationDate();
    int const getPhotoFilesFoundContainingEXIFWODate();
    ~Scanner();

private slots:
    void processScan();

public slots:
    void scan(const std::string& directoryPath, bool includeSubdirectories);
    void resetScanner();
    void handleTimeout();

signals:
    void filesFoundUpdated(int filesFound);
    void scanCompleted();

private:
    void scanDirectory(const std::string& directoryPath, bool includeSubdirectories);
    void emitPeriodically();
    std::string escapeBackslashes(const std::string& input);
    std::atomic<int> filesFound{0};
    std::atomic<int> photoFilesFoundContainingEXIFData{0};
    std::atomic<int> photoFilesFoundContainingValidCreationDate{0};
    std::atomic<int> photoFilesFoundContainingEXIFWODate{0};
    std::vector<std::unique_ptr<BasicFileHandler>> basicFileHandlers;
    std::vector<std::unique_ptr<PhotoFileHandler>> photoFileHandlers;
    std::vector<std::unique_ptr<PhotoFileHandler>> invalidPhotoFileHandlers;
    std::vector<std::unique_ptr<VideoFileHandler>> videoFileHandlers;
    std::mutex mutex;

    FileFactory fileFactory;
    QTimer* progressTimer;
    std::string directoryPath;
    bool includeSubdirectories;
};

#endif // SCANNER_H
