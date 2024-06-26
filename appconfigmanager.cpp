/***********************************************************************
 * File Name: appconfigmanager.cpp
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Implementation of AppConfigManager. Includes methods to
 *              save and load configuration settings from a file, validate
 *              the configuration's viability based on directory existence,
 *              and handle file paths for configuration storage. It works
 *              closely with the AppConfig singleton to manage application
 *              settings comprehensively.
 * License: MIT License
 ***********************************************************************/

#define NOMINMAX
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <cstddef>
#include <QDir>
#include <QMessageBox>
#include "appconfigmanager.h"

// Windows-specific includes should come after C++ standard library includes
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

// AppConfig is a singleton - use config to address and use it
AppConfig* AppConfig::ptrInstance = nullptr;

// Constructor initializes with a reference to AppConfig instance
AppConfigManager::AppConfigManager(AppConfig& config) : config(config) {}

// Validates the source directory existence, optionally showing a message box
bool AppConfigManager::scanConfigurationValid(bool showMessage){
    if(!checkDirectoryExists(showMessage, config.getSourceDirectory(), "Source")) return false;
    return true;
}

bool AppConfigManager::copyConfigurationValid(bool showMessage){
    if(config.getMoveInvalidFileMeta()){
        if(!checkDirectoryExists(showMessage, config.getInvalidFileMetaDirectory(), "Invalid File Meta")) return false;
    }
    if(config.getDuplicatesFoundSelection() == "Move To Folder"){
        if(!checkDirectoryExists(showMessage, config.getDuplicatesDirectory(), "Invalid Duplicates")) return false;
    }
    return true;
}

bool AppConfigManager::checkDirectoryExists(bool showMessage,
                                            std::string directoryPath,
                                            std::string directoryType){
    if(!QDir(QString::fromStdString(directoryPath)).exists() || directoryPath == ""){
        if(showMessage){
            std::string errorMessage = "Directory does not exist. Please correct this.";
            if (directoryType != "") errorMessage = directoryType + " Directory does not exist. Please correct this.";
            QMessageBox::critical(nullptr,
                                  "Error",
                                  QString::fromStdString(errorMessage),
                                  QMessageBox::Ok);
        }
        return false;
    }
    return true;
}

// Returns the path to the executable, handling differences between Windows and POSIX systems
std::string AppConfigManager::getExecutablePath() {
    char path[1024] = { 0 }; // Adjust size as needed
#ifdef _WIN32
    GetModuleFileNameA(NULL, path, sizeof(path));
    std::string pathStr = std::string(path);
    return pathStr.substr(0, pathStr.find_last_of("\\/"));
#else
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
    if (count != -1) {
        std::string pathStr = std::string(path, count);
        return pathStr.substr(0, pathStr.find_last_of("\\/"));
    }
    return "";
#endif
}

std::string AppConfigManager::getDefaultConfigPath() {
    return getExecutablePath() + "/config.dat"; // default config file name
}

// Saves the current configuration to the specified file path
void AppConfigManager::save(const std::string& filePath) {
    std::ofstream outFile(filePath);
    AppConfig& config = AppConfig::get(); // Access the singleton instance
    if (outFile.is_open()) {
        outFile << config.getSourceDirectory() << std::endl;
        outFile << config.getOutputDirectory() << std::endl;
        outFile << config.getInvalidFileMetaDirectory() << std::endl;
        outFile << config.getDuplicatesDirectory() << std::endl;
        outFile << config.getDuplicatesFoundSelection() << std::endl;
        outFile << config.getPhotosOutputFolderStructureSelection() << std::endl;
        outFile << config.getPhotosDuplicateIdentitySetting() << std::endl;
        outFile << config.getMoveInvalidFileMeta() << std::endl;
        outFile << config.getIncludeSubDirectories() << std::endl;
        outFile << config.getPhotosReplaceDashesWithUnderscores() << std::endl;
        outFile.close();
        std::clog << "Configuration saved to: " << filePath << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

// Loads configuration settings from the specified file path
bool AppConfigManager::load(const std::string& filePath) {
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        AppConfig& config = AppConfig::get(); // Access the singleton instance
        std::string sourceDir, outputDir, invalidMetaDir, duplicatesDir,
            duplicatesSelection, folderStructureSelection, photoDuplicateIdentitySetting;
        bool moveInvalidMeta, includeSubDirs, photosReplaceDashesWithUnderscores;

        getline(inFile, sourceDir);
        getline(inFile, outputDir);
        getline(inFile, invalidMetaDir);
        getline(inFile, duplicatesDir);
        getline(inFile, duplicatesSelection);
        getline(inFile, folderStructureSelection);
        getline(inFile, photoDuplicateIdentitySetting);
        inFile >> moveInvalidMeta;
        inFile >> includeSubDirs;
        inFile >> photosReplaceDashesWithUnderscores;

        config.setSourceDirectory(sourceDir);
        config.setOutputDirectory(outputDir);
        config.setInvalidFileMetaDirectory(invalidMetaDir);
        config.setDuplicatesDirectory(duplicatesDir);
        config.setMoveInvalidFileMeta(moveInvalidMeta);
        config.setIncludeSubDirectories(includeSubDirs);
        config.setDuplicatesFoundSelection(duplicatesSelection);
        config.setPhotosOutputFolderStructureSelection(folderStructureSelection);
        config.setPhotosDuplicateIdentitySetting(photoDuplicateIdentitySetting);
        config.setPhotosReplaceDashesWithUnderscores(photosReplaceDashesWithUnderscores);

        std::clog << "Configuration loaded from to: " << filePath << std::endl;

        // Clear potential newline left in the buffer
        inFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        inFile.close();
        return true;
    } else {
        std::cerr << "Unable to open file for reading." << std::endl;
        return false;
    }
}
