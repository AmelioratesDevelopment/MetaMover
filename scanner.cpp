#include "scanner.h"
#include <filesystem>
#include <QDebug>
#include <QDir>
#include <QString>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>

Scanner::Scanner(QObject* parent) : QObject(parent), stopTimer(false) {}

Scanner::~Scanner() {}

void Scanner::scan(const std::string& directoryPath, bool includeSubdirectories) {
    filesFound = 0;
    photoFilesFoundContainingEXIFData = 0;
    photoFilesFoundContainingValidCreationDate = 0;
    photoFilesFoundContainingEXIFWODate = 0;
    resetScanner();
    stopTimer = false;
    std::lock_guard<std::mutex> lock(mutex);
    // Start timer thread
    std::thread timerThread(&Scanner::emitPeriodically, this);

    scanDirectory(directoryPath, includeSubdirectories);

    // Stop timer thread and ensure final emit
    stopTimer = true;
    timerThread.join();
    emit filesFoundUpdated(filesFound.load());
    emit scanCompleted();
}

void Scanner::scanDirectory(const std::string& directoryPath, bool includeSubdirectories) {
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::string path = QString(QDir::toNativeSeparators(QString::fromStdString(entry.path().string()))).toStdString();
        //std::replace(path.begin(), path.end(), '\\', '/');
        path = escapeBackslashes(path);
        if (entry.is_directory() && includeSubdirectories) {
            scanDirectory(path, true);
        } else if (!entry.is_directory()) {
            auto handler = fileFactory.makeFileHandler(path);
            // Use dynamic_cast to check the actual derived type of the handler
            if (auto* pVideoHandler = dynamic_cast<VideoFileHandler*>(handler.get())) {
                videoFileHandlers.push_back(std::unique_ptr<VideoFileHandler>(pVideoHandler));
            } else if (auto* pPhotoHandler = dynamic_cast<PhotoFileHandler*>(handler.get())) {
                if(pPhotoHandler->containsEXIFData) photoFilesFoundContainingEXIFData++;
                if(pPhotoHandler->validCreationDataInEXIF) photoFilesFoundContainingEXIFData++;
                if(pPhotoHandler->hasEXIFDateWODate) photoFilesFoundContainingEXIFWODate++;
                photoFileHandlers.push_back(std::unique_ptr<PhotoFileHandler>(pPhotoHandler));
            } else if (auto* pBasicHandler = dynamic_cast<BasicFileHandler*>(handler.get())) { // fallback to the basic type
                basicFileHandlers.push_back(std::unique_ptr<BasicFileHandler>(pBasicHandler));
            } else {
                std::cout << "Unknown handler type for file: " << path << std::endl;
            }

            handler.release();
            filesFound++;
        }
    }
}

std::string Scanner::escapeBackslashes(const std::string& input) {
    std::string output = input;
    size_t position = 0;
    while ((position = output.find("\\", position)) != std::string::npos) {
        output.replace(position, 1, "\\\\");
        position += 2;  // Move past the inserted backslash
    }
    return output;
}

void Scanner::resetScanner()
{
    basicFileHandlers.clear();
    photoFileHandlers.clear();
    videoFileHandlers.clear();
}

int const Scanner::getTotalFilesFound()
{
    int total = 0;
    total += static_cast<int>(basicFileHandlers.size());
    total += static_cast<int>(photoFileHandlers.size());
    total += static_cast<int>(videoFileHandlers.size());
    return total;
}

int const Scanner::getTotalPhotoFilesFound()
{
    return static_cast<int>(photoFileHandlers.size());
}

int const Scanner::getPhotoFilesFoundContainingEXIFData()
{
    return photoFilesFoundContainingEXIFData;
}

int const Scanner::getPhotoFilesFoundContainingValidCreationDate()
{
    return photoFilesFoundContainingValidCreationDate;
}

int const Scanner::getPhotoFilesFoundContainingEXIFWODate()
{
    return photoFilesFoundContainingEXIFWODate;
}



void Scanner::emitPeriodically() {
    while (!stopTimer) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        emit filesFoundUpdated(filesFound.load());
    }
}
