#ifndef APPCONFIGMANAGER_H
#define APPCONFIGMANAGER_H

#include <string>
#include "appconfig.h"

/*****************************
 * Author: Blake Azuela
 *
 * This class is meant to facilitate
 * the CRUD of the Application
 * Configuration file.
 *
 * *************************/

class AppConfigManager
{
private:
    AppConfig& config;  // Reference to AppConfig instance

public:
    explicit AppConfigManager(AppConfig& config);
    static std::string getExecutablePath();
    static std::string getDefaultConfigPath();
    void SetSourceDirectory(std::string sourcePath);
    void SetSourceIncludeSubDirsFlag(bool enabled);
    void SetSourceIfMetaInvalidMoveFlag(bool enabled);
    void SetPhotosReplaceDashesWithUnderscores(bool enabled);
    void SetPhotosDuplicateIdentitySetting(std::string selection);
    void SetSourceIfMetaInvalidMoveDirectory(std::string selectedFolder);
    void SetOutputDirectory(std::string outputPath);
    void SetOutputIfDupesFound(std::string selection);
    void SetOutputDupesDirectory(std::string selectedFolder);
    void SetDuplicatesSelection(std::string selection);
    void SetPhotoOutputFolderStructureSelection(std::string selection);
    std::string GetSourceDirectory();
    bool GetSourceIncludeSubDirsFlag();
    bool GetSourceIfMetaInvalidMoveFlag();
    bool GetPhotosReplaceDashesWithUnderscores();
    std::string GetPhotosDuplicateIdentitySetting();
    std::string GetSourceIfMetaInvalidMoveDirectory();
    std::string GetOutputDirectory();
    std::string GetOutputIfDupesFound();
    std::string GetOutputDupesDirectory();
    std::string GetDuplicatesSelection();
    std::string GetPhotoOutputFolderStructureSelection();
    std::vector<std::string> GetDuplicatesFoundOptions();
    std::vector<std::string> GetMediaOutputFolderStructureOptions();
    void save(const std::string& filePath = getDefaultConfigPath());
    bool load(const std::string& filePath = getDefaultConfigPath());
};

#endif // APPCONFIGMANAGER_H
