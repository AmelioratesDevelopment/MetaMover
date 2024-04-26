#define NOMINMAX
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <cstddef>
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

AppConfigManager::AppConfigManager(AppConfig& config) : config(config) {}


// Getter and Setter - Source Directory
void AppConfigManager::SetSourceDirectory(std::string sourcePath){
    config.setSourceDirectory(sourcePath);
}

std::string AppConfigManager::GetSourceDirectory(){
    return config.getSourceDirectory();
}

// Getter and Setter - Source Directory Include SubDirs
void AppConfigManager::SetSourceIncludeSubDirsFlag(bool enabled){
    config.setIncludeSubDirectories(enabled);
}

bool AppConfigManager::GetSourceIncludeSubDirsFlag(){
    return config.getIncludeSubDirectories();
}

// Getter and Setter - Source Directory If Meta Invalid Move Flag
void AppConfigManager::SetSourceIfMetaInvalidMoveFlag(bool enabled){
    config.setMoveInvalidFileMeta(enabled);
}

bool AppConfigManager::GetSourceIfMetaInvalidMoveFlag(){
    return config.getMoveInvalidFileMeta();
}

// Getter and Setter - Source Directory If Meta Invalid Move Directory
void AppConfigManager::SetSourceIfMetaInvalidMoveDirectory(std::string selectedFolder){
    config.setInvalidFileMetaDirectory(selectedFolder);
}

std::string AppConfigManager::GetSourceIfMetaInvalidMoveDirectory(){
    return config.getInvalidFileMetaDirectory();
}

// Getter and Setter - Output Directory
void AppConfigManager::SetOutputDirectory(std::string outputPath){
    config.setOutputDirectory(outputPath);
}

std::string AppConfigManager::GetOutputDirectory(){
    return config.getOutputDirectory();
}

// Getter and Setter - Output Directory
void AppConfigManager::SetOutputIfDupesFound(std::string selection){
    config.setDuplicatesFoundSelection(selection);
}

std::string AppConfigManager::GetDuplicatesSelection(){
    return config.getDuplicatesFoundSelection();
}

// Getter and Setter - Output Duplicates Directory
void AppConfigManager::SetOutputDupesDirectory(std::string selectedFolder){
    config.setDuplicatesDirectory(selectedFolder);
}

std::string AppConfigManager::GetOutputDupesDirectory(){
    return config.getDuplicatesDirectory();
}

// Getter and Setter - Photo Selection
void AppConfigManager::SetPhotoOutputFolderStructureSelection(std::string selection){
    config.setPhotosOutputFolderStructureSelection(selection);
}

std::string AppConfigManager::GetPhotoOutputFolderStructureSelection(){
    return config.getPhotosOutputFolderStructureSelection();
}

// Getter and Setter - Photo Selection
void AppConfigManager::SetDuplicatesSelection(std::string selection){
    config.setDuplicatesFoundSelection(selection);
}

// Getter and Setter - Photo Replace Dashes With Underscores
void AppConfigManager::SetPhotosReplaceDashesWithUnderscores(bool enabled){
    config.setPhotosReplaceDashesWithUnderscores(enabled);
}

bool AppConfigManager::GetPhotosReplaceDashesWithUnderscores(){
    return config.getPhotosReplaceDashesWithUnderscores();
}

// Getter and Setter - Photo Duplicates Indentifcation Setting
void AppConfigManager::SetPhotosDuplicateIdentitySetting(std::string selection){
    config.setPhotosDuplicateIdentitySetting(selection);
}

std::string AppConfigManager::GetPhotosDuplicateIdentitySetting(){
    return config.getPhotosDuplicateIdentitySetting();
}

// Getter - Provide options when duplicates were found
std::vector<std::string> AppConfigManager::GetDuplicatesFoundOptions(){
    return config.getDuplicatesFoundOptions();
}

// Getter - Provide media output folder structure options
std::vector<std::string> AppConfigManager::GetMediaOutputFolderStructureOptions(){
    return config.getMediaOutputFolderStructureOptions();
}

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
