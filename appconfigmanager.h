#ifndef APPCONFIGMANAGER_H
#define APPCONFIGMANAGER_H

/***********************************************************************
 * File Name: appconfigmanager.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the AppConfigManager class, which manages
 *              CRUD operations for application configuration settings. This
 *              class acts as a bridge to the AppConfig singleton, providing
 *              functionality for loading and saving configuration data from
 *              and to a file, validating the current configuration, and
 *              retrieving default configuration paths.
 * License: MIT License
 ***********************************************************************/

#include <string>
#include "appconfig.h"

class AppConfigManager
{

public:
    AppConfig& config; // Reference to AppConfig instance
    explicit AppConfigManager(AppConfig& config);
    bool scanConfigurationValid(bool showMessage);
    static std::string getExecutablePath();
    static std::string getDefaultConfigPath();
    void save(const std::string& filePath = getDefaultConfigPath());
    bool load(const std::string& filePath = getDefaultConfigPath());
};

#endif // APPCONFIGMANAGER_H
