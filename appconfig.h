#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>

/*****************************
 * Author: Blake Azuela
 *
 * This class provide a model to store and operate
 * on the application configuration. Because there
 * should really only be one AppConfig class during
 * runtime this is a good example implementation of
 * the singleton design pattern. I would also like
 * to conceed that singletons are sometimes best
 * replaced with namespaces.
 *
 * *************************/

class AppConfig {
private:
    static AppConfig* ptrInstance;

    // Constructor and Destructor
    AppConfig() : sourceDirectory(""), outputDirectory(""), invalidFileMetaDirectory(""),
        duplicatesDirectory(""), moveInvalidFileMeta(false), includeSubDirectories(false),
        duplicatesFoundSelection(0), photosOutputFolderStructureSelection(0) {}
    ~AppConfig() {}

    // Configuration Variables
    std::string sourceDirectory;
    std::string outputDirectory;
    std::string invalidFileMetaDirectory;
    std::string duplicatesDirectory;
    bool moveInvalidFileMeta;
    bool includeSubDirectories;
    int duplicatesFoundSelection;
    int photosOutputFolderStructureSelection;

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

    // Getters and Setters for each member variable
    std::string getSourceDirectory() const { return sourceDirectory; }
    void setSourceDirectory(const std::string &value) { sourceDirectory = value; }

    std::string getOutputDirectory() const { return outputDirectory; }
    void setOutputDirectory(const std::string &value) { outputDirectory = value; }

    std::string getInvalidFileMetaDirectory() const { return invalidFileMetaDirectory; }
    void setInvalidFileMetaDirectory(const std::string &value) { invalidFileMetaDirectory = value; }

    std::string getDuplicatesDirectory() const { return duplicatesDirectory; }
    void setDuplicatesDirectory(const std::string &value) { duplicatesDirectory = value; }

    bool getMoveInvalidFileMeta() const { return moveInvalidFileMeta; }
    void setMoveInvalidFileMeta(bool value) { moveInvalidFileMeta = value; }

    bool getIncludeSubDirectories() const { return includeSubDirectories; }
    void setIncludeSubDirectories(bool value) { includeSubDirectories = value; }

    int getDuplicatesFoundSelection() const { return duplicatesFoundSelection; }
    void setDuplicatesFoundSelection(int value) { duplicatesFoundSelection = value; }

    int getPhotosOutputFolderStructureSelection() const { return photosOutputFolderStructureSelection; }
    void setPhotosOutputFolderStructureSelection(int value) { photosOutputFolderStructureSelection = value; }
};

#endif // APPCONFIG_H
