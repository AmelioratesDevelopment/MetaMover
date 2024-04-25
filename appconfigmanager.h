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
    void SetOutputDirectory(std::string outputPath);
    std::string GetOutputDirectory();
    std::string GetSourceDirectory();
    void save(const std::string& filePath = getDefaultConfigPath());
    bool load(const std::string& filePath = getDefaultConfigPath());
};

#endif // APPCONFIGMANAGER_H
