#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <string>

/***********************************************************************
 * File Name: ifilehandler.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the IFileHandler interface. This interface
 *              establishes the standard operations for file handling within
 *              the application, ensuring that all file handler types implement
 *              the necessary processFile method. It serves as the foundation
 *              for creating specific file handlers for different file types,
 *              such as basic files, photos, and potentially videos.
 * License: MIT License
 ***********************************************************************/


class IFileHandler{
public:
    virtual void processFile() = 0;
    virtual std::string getSourceFilePath() = 0;
    virtual std::string getSourceFileName() = 0;
    virtual std::string getTargetFileName() = 0;
    virtual void setTargetFileName(std::string targetFileName) = 0;
    virtual ~IFileHandler() = default;
};

#endif // IFILEHANDLER_H
