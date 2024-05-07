#ifndef BASICFILEHANDLER_H
#define BASICFILEHANDLER_H

/***********************************************************************
 * File Name: basicfilehandler.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the BasicFileHandler class, which implements
 *              the IFileHandler interface. This class serves as the base class
 *              for handling generic files, providing foundational file processing
 *              functionalities. It is extended by specialized handlers for photos
 *              and videos to incorporate specific processing needs.
 * License: MIT License
 ***********************************************************************/


#include "ifilehandler.h"
#include <string>


class BasicFileHandler : public IFileHandler {
protected:
    std::string filePath;


public:
    BasicFileHandler(const std::string inputFilePath);
    virtual ~BasicFileHandler();
    virtual void processFile() override;

private:
    BasicFileHandler() = delete;
};

#endif // BASICFILEHANDLER_H
