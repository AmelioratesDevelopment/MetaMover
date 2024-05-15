/***********************************************************************
 * File Name: basicfilehandler.cpp
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Implementation of the BasicFileHandler class. This file
 *              includes basic operations to process generic files, primarily
 *              logging the file processing. It serves as a foundation for more
 *              specialized file handlers.
 * License: MIT License
 ***********************************************************************/
#include <iostream>
#include <filesystem>
#include <string>
#include <QString>
#include <QFileInfo>
#include "basicfilehandler.h"

BasicFileHandler::BasicFileHandler(const std::string inputFilePath)
    : filePath(inputFilePath) {
}

BasicFileHandler::~BasicFileHandler() {
}

std::string BasicFileHandler::getSourceFilePath(){
    return filePath;
}

std::string BasicFileHandler::getSourceFileName() {
    QFileInfo fileInfo(QString::fromStdString(getSourceFilePath()));
    return fileInfo.fileName().toStdString();
}


std::string BasicFileHandler::getTargetFileName() {
    return targetFileName;
}

void BasicFileHandler::setTargetFileName(std::string targetFileName) {
    if (targetFileName.empty()) {
        targetFileName = getSourceFileName();
    }

    std::filesystem::path path(targetFileName);
    std::string currentExtension = path.extension().string();
    std::string sourceExtension = std::filesystem::path(getSourceFilePath()).extension().string();

    if (currentExtension != sourceExtension) {
        // If there is no extension in the target, append the source extension
        if (currentExtension.empty()) {
            targetFileName += sourceExtension;
        } else {
            // Replace the wrong extension with the correct one
            targetFileName.replace(targetFileName.length() - currentExtension.length(), currentExtension.length(), sourceExtension);
        }
    }

    // Handle directory path
    if (!path.has_parent_path()) {
        // If no directory is specified in the targetFileName, return only the filename
        this->targetFileName = path.filename().string();
    } else {
        // If a directory is specified in targetFileName, use it
        this->targetFileName = targetFileName;
    }
}

void BasicFileHandler::processFile(){
    std::cout << "Processing a basic file: " << this->filePath << std::endl;
    setTargetFileName();
}
