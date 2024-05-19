/***********************************************************************
 * File Name: Scanner.cpp
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Implementation of the Scanner class. Provides functionality
 *              to scan directories recursively and manage file handlers based
 *              on file type, extracting and processing EXIF and other metadata.
 *              Includes mechanisms to update the UI periodically with scan
 *              progress and completion status.
 * License: MIT License
 ***********************************************************************/

#include <QDir>
#include <QString>
#include <QMessageBox>
#include <filesystem>
#include <iostream>
#include "scanner.h"

Scanner::Scanner(QObject* parent)
    : QObject(parent) {}

Scanner::~Scanner() {}

void Scanner::scan(const std::string& dirPath, bool includeSubdirs) {
    resetScanner();
    cancelScan = false;
    scanRunning = true;
    scanDirectory(dirPath, includeSubdirs);
    scanRunning = false;
    emit scanCompleted();
}

void Scanner::scanDirectory(const std::string& directoryPath, bool includeSubdirectories) {
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (cancelScan) {
            resetScanner();
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
                    photoFilesUnsupportedFound++;
                    invalidPhotoFileHandlers.push_back(std::unique_ptr<PhotoFileHandler>(pPhotoHandler));
                    handler.release();
                    filesFound++;
                    continue;
                } else {
                    photoFilesFoundContainingEXIFData++;
                }
                if (!pPhotoHandler->validCreationDataInEXIF) {
                    photoFilesUnsupportedFound++;
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
                photoFilesUnsupportedFound++;
                filesFound++;
            } else {
                std::cout << "Unknown handler type for file: " << path << std::endl;
                photoFilesUnsupportedFound++;
            }
            handler.release();
        }
    }
}

void Scanner::resetScanner() {
    filesFound = 0;
    photoFilesFoundContainingEXIFData = 0;
    photoFilesFoundContainingValidCreationDate = 0;
    photoFilesUnsupportedFound = 0;
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

int const Scanner::getPhotoFilesUnsupportedFiles() {
    return photoFilesUnsupportedFound.load();
}
