#ifndef VIDEOFILEHANDLER_H
#define VIDEOFILEHANDLER_H

/***********************************************************************
 * File Name: videofilehandler.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the VideoFileHandler class, which extends
 *              BasicFileHandler. Intended for processing video files if
 *              developed, following similar processing patterns to other
 *              file handlers.
 * License: MIT License
 ***********************************************************************/

#include <string>
#include "basicfilehandler.h"

class VideoFileHandler : public BasicFileHandler {
protected:

public:
    VideoFileHandler(const std::string inputFilePath);
    virtual ~VideoFileHandler();
    virtual void processFile() override;

private:
    VideoFileHandler() = delete;
};
#endif // VIDEOFILEHANDLER_H
