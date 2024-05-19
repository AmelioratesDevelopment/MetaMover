#include "scanner.h"
#include <filesystem>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QMessageBox>
#include <atomic>
#include <iostream>

Scanner::Scanner(QObject* parent)
    : QObject(parent), progressTimer(new QTimer(this)) {
    connect(progressTimer, &QTimer::timeout, this, &Scanner::handleTimeout);
}

Scanner::~Scanner() {
    if (progressTimer->isActive()) {
        progressTimer->stop();
    }
    delete progressTimer;
}

void Scanner::scan(const std::string& dirPath, bool includeSubdirs) {
    directoryPath = dirPath;
    includeSubdirectories = includeSubdirs;

    filesFound = 0;
    photoFilesFoundContainingEXIFData = 0;
    photoFilesFoundContainingValidCreationDate = 0;
    photoFilesFoundContainingEXIFWODate = 0;
    resetScanner();
    cancelScan = false;

    progressTimer->start(200); // Start the timer to emit progress every 200 milliseconds
    processScan();
}

void Scanner::processScan() {
    scanDirectory(directoryPath, includeSubdirectories);
    progressTimer->stop();
    emit filesFoundUpdated(filesFound.load());
    emit scanCompleted();
}

void Scanner::scanDirectory(const std::string& directoryPath, bool includeSubdirectories) {
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (cancelScan) {
            return;
        }
        std::string path = QString(QDir::toNativeSeparators(QString::fromStdString(entry.path().string()))).toStdString();
        if (entry.is_directory() && includeSubdirectories) {
            scanDirectory(path, true);
        } else if (!entry.is_directory()) {
            auto handler = fileFactory.makeFileHandler(path);
            if (auto* pVideoHandler = dynamic_cast<VideoFileHandler*>(handler.get())) {
                videoFileHandlers.push_back(std::unique_ptr<VideoFileHandler>(pVideoHandler));
            } else if (auto* pPhotoHandler = dynamic_cast<PhotoFileHandler*>(handler.get())) {
                if (!pPhotoHandler->containsEXIFData) {
                    photoFilesFoundContainingEXIFWODate++;
                    invalidPhotoFileHandlers.push_back(std::unique_ptr<PhotoFileHandler>(pPhotoHandler));
                    handler.release();
                    filesFound++;
                    continue;
                } else {
                    photoFilesFoundContainingEXIFData++;
                }
                if (!pPhotoHandler->validCreationDataInEXIF) {
                    photoFilesFoundContainingEXIFWODate++;
                    invalidPhotoFileHandlers.push_back(std::unique_ptr<PhotoFileHandler>(pPhotoHandler));
                    handler.release();
                    filesFound++;
                    continue;
                } else {
                    photoFilesFoundContainingValidCreationDate++;
                    photoFileHandlers.push_back(std::unique_ptr<PhotoFileHandler>(pPhotoHandler));
                    filesFound++;
                }
            } else if (auto* pBasicHandler = dynamic_cast<BasicFileHandler*>(handler.get())) {
                basicFileHandlers.push_back(std::unique_ptr<BasicFileHandler>(pBasicHandler));
                photoFilesFoundContainingEXIFWODate++;
                filesFound++;
            } else {
                std::cout << "Unknown handler type for file: " << path << std::endl;
                photoFilesFoundContainingEXIFWODate++;
            }
            handler.release();
        }
    }
}

void Scanner::resetScanner() {
    basicFileHandlers.clear();
    photoFileHandlers.clear();
    videoFileHandlers.clear();
    invalidPhotoFileHandlers.clear();
}

bool Scanner::checkScanResults(bool showMessage) {
    if (getTotalFilesFound() <= 0) {
        if (showMessage) {
            QMessageBox::critical(nullptr, "Error", "No Files Found in Scan.", QMessageBox::Ok);
        }
        return false;
    }
    return true;
}

std::vector<std::unique_ptr<PhotoFileHandler>>& Scanner::getPhotoFileHandlers() {
    return photoFileHandlers;
}

std::vector<std::unique_ptr<PhotoFileHandler>>& Scanner::getInvalidPhotoFileHandlers() {
    return invalidPhotoFileHandlers;
}

int const Scanner::getTotalFilesFound() {
    int total = 0;
    total += static_cast<int>(basicFileHandlers.size());
    total += static_cast<int>(photoFileHandlers.size());
    total += static_cast<int>(videoFileHandlers.size());
    return total;
}

int const Scanner::getTotalPhotoFilesFound() {
    return static_cast<int>(photoFileHandlers.size());
}

int const Scanner::getPhotoFilesFoundContainingEXIFData() {
    return photoFilesFoundContainingEXIFData.load();
}

int const Scanner::getPhotoFilesFoundContainingValidCreationDate() {
    return photoFilesFoundContainingValidCreationDate.load();
}

int const Scanner::getPhotoFilesFoundContainingEXIFWODate() {
    return photoFilesFoundContainingEXIFWODate.load();
}

void Scanner::handleTimeout() {
    emit filesFoundUpdated(filesFound.load());
}
