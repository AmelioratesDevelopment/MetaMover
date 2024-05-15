#ifndef APPCONFIG_H
#define APPCONFIG_H

/***********************************************************************
 * File Name: appconfig.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the AppConfig class, designed as a singleton
 *              to maintain a single instance of application configuration data.
 *              This class provides a central point of access and management for
 *              all configurable aspects of the application, such as directories,
 *              file handling preferences, and UI settings, ensuring consistent
 *              application behavior. The singleton pattern is chosen to prevent
 *              multiple instantiations and provide global access.
 * License: MIT License
 ***********************************************************************/


#include <string>
#include <vector>
#include <QString>
#include <QDir>

class AppConfig {
private:
    static AppConfig* ptrInstance;

    // Constructor and Destructor
    AppConfig() : sourceDirectory(""), outputDirectory(""), invalidFileMetaDirectory(""),
        duplicatesDirectory(""), duplicatesFoundSelection(""), photosOutputFolderStructureSelection(""),
        moveInvalidFileMeta(false), includeSubDirectories(false) {
        duplicatesFoundOptions = {
            "Add 'Copy##' and Move/Copy",
            "Do Not Move or Copy",
            "Overwrite",
            "Move To Folder"
        };
        mediaOutputFolderStructureOptions = {
            "Year",
            "Month",
            "Day",
            "Year, Month",
            "Year, Month, Day",
            "Year, Month, Day, Camera Model",
            "Camera Model, Year",
            "Camera Model, Year, Month",
            "Camera Model, Year, Month, Day"
        };
    }
    ~AppConfig() {}

    // Configuration Variables
    std::string sourceDirectory;
    std::string outputDirectory;
    std::string invalidFileMetaDirectory;
    std::string duplicatesDirectory;
    std::string duplicatesFoundSelection;
    bool moveInvalidFileMeta;
    bool includeSubDirectories;

    //Options - Photo Specific
    std::string photosOutputFolderStructureSelection;
    std::string photosDuplicateIdentitiySetting;
    bool photosReplaceDashesWithUnderscores;

    // Vector to store options for handling duplicates
    std::vector<std::string> duplicatesFoundOptions;
    // Vector to store options for handling media folder stucture config
    std::vector<std::string> mediaOutputFolderStructureOptions;

public:

    // Singleton methods
    AppConfig(AppConfig const&) = delete;
    void operator=(AppConfig const&) = delete;

    static AppConfig& get() {
        if (!ptrInstance) {
            ptrInstance = new AppConfig();
        }
        return *ptrInstance;
    }

    static void destroy() {
        delete ptrInstance;
        ptrInstance = nullptr;
    }

    std::string convertToNativePath(std::string value) const {
        return QString(QDir::toNativeSeparators(QString::fromStdString(value))).toStdString();
    }

    // Getters and Setters for each member variable
    std::string getSourceDirectory() const { return convertToNativePath(sourceDirectory); }
    void setSourceDirectory(const std::string &value) { sourceDirectory = value; }

    std::string getOutputDirectory() const { return convertToNativePath(outputDirectory); }
    void setOutputDirectory(const std::string &value) { outputDirectory = value; }

    std::string getInvalidFileMetaDirectory() const { return convertToNativePath(invalidFileMetaDirectory); }
    void setInvalidFileMetaDirectory(const std::string &value) { invalidFileMetaDirectory = value; }

    std::string getDuplicatesDirectory() const { return convertToNativePath(duplicatesDirectory); }
    void setDuplicatesDirectory(const std::string &value) { duplicatesDirectory = value; }

    std::string getDuplicatesFoundSelection() const { return duplicatesFoundSelection; }
    void setDuplicatesFoundSelection(std::string &value) { duplicatesFoundSelection = value; }

    std::string getPhotosOutputFolderStructureSelection() const { return photosOutputFolderStructureSelection; }
    void setPhotosOutputFolderStructureSelection(std::string &value) { photosOutputFolderStructureSelection = value; }

    std::string getPhotosDuplicateIdentitySetting() const { return photosDuplicateIdentitiySetting; }
    void setPhotosDuplicateIdentitySetting(std::string &value) { photosDuplicateIdentitiySetting = value; }

    bool getMoveInvalidFileMeta() const { return moveInvalidFileMeta; }
    void setMoveInvalidFileMeta(bool value) { moveInvalidFileMeta = value; }

    bool getIncludeSubDirectories() const { return includeSubDirectories; }
    void setIncludeSubDirectories(bool value) { includeSubDirectories = value; }

    bool getPhotosReplaceDashesWithUnderscores() const { return photosReplaceDashesWithUnderscores; }
    void setPhotosReplaceDashesWithUnderscores(bool value) { photosReplaceDashesWithUnderscores = value; }

    const std::vector<std::string>& getDuplicatesFoundOptions() const { return duplicatesFoundOptions; }
    const std::vector<std::string>& getMediaOutputFolderStructureOptions() const { return mediaOutputFolderStructureOptions; }
};

#endif // APPCONFIG_H
