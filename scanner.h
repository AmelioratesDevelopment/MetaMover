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
#include "basicfilehandler.h"
#include "filehandlerfactory.h"

class Scanner : public QObject {
    Q_OBJECT

public:
    explicit Scanner(QObject* parent = nullptr);
    bool checkScanResults(bool showMessage);
    void resetScanner();
    std::vector<std::unique_ptr<PhotoFileHandler>>& getPhotoFileHandlers();
    std::vector<std::unique_ptr<PhotoFileHandler>>& getInvalidPhotoFileHandlers();
    int const getTotalFilesFound();
    int const getTotalPhotoFilesFound();
    int const getPhotoFilesFoundContainingEXIFData();
    int const getPhotoFilesFoundContainingValidCreationDate();
    int const getPhotoFilesUnsupportedFiles();
    std::atomic<bool> cancelScan{false};
    std::atomic<bool> scanRunning{false};
    ~Scanner();    

public slots:
    void scan(const std::string& directoryPath, bool includeSubdirectories);

signals:
    void scanCompleted();

private:
    void scanDirectory(const std::string& directoryPath, bool includeSubdirectories);
    void processScan();
    std::atomic<int> filesFound{0};
    std::atomic<int> photoFilesFoundContainingEXIFData{0};
    std::atomic<int> photoFilesFoundContainingValidCreationDate{0};
    std::atomic<int> photoFilesUnsupportedFound{0};
    std::vector<std::unique_ptr<BasicFileHandler>> basicFileHandlers;
    std::vector<std::unique_ptr<PhotoFileHandler>> photoFileHandlers;
    std::vector<std::unique_ptr<PhotoFileHandler>> invalidPhotoFileHandlers;
    std::vector<std::unique_ptr<VideoFileHandler>> videoFileHandlers;
    FileFactory fileFactory;
};

#endif // SCANNER_H
