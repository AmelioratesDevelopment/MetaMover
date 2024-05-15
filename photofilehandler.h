#ifndef PHOTOFILEHANDLER_H
#define PHOTOFILEHANDLER_H

/***********************************************************************
 * File Name: photofilehandler.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the PhotoFileHandler class, derived from
 *              BasicFileHandler. This class specifically handles image files,
 *              extracting EXIF data and evaluating it for metadata validity,
 *              including the original creation date and camera model information.
 * License: MIT License
 ***********************************************************************/


#include <string>
#include <chrono>
#include "basicfilehandler.h"
#include "exif.h"

class PhotoFileHandler : public BasicFileHandler {
protected:

public:
    PhotoFileHandler(const std::string inputFilePath);
    virtual ~PhotoFileHandler();
    virtual void processFile() override;
    bool fileValid;
    bool containsEXIFData;
    bool validCreationDataInEXIF;
    std::chrono::system_clock::time_point getOriginalDateTime();
    std::chrono::time_point<std::chrono::system_clock> getFileCreationTime() const;
    std::string getCameraModel();
    easyexif::EXIFInfo getExifData();
    bool overwriteEnabled;

private:
    void parseDateTime(const std::string& dateTimeStr);
    void extractEXIFData();
    std::chrono::system_clock::time_point originalDateTime;
    std::string cameraModel;
    easyexif::EXIFInfo exifData;
    PhotoFileHandler() = delete;
};
#endif // PHOTOFILEHANDLER_H
