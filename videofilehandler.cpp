/***********************************************************************
 * File Name: videofilehandler.cpp
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Implementation of the VideoFileHandler class, which might
 *              handle video files by processing specific metadata related
 *              to videos. Currently serves as a stub with potential for
 *              future development.
 * License: MIT License
 ***********************************************************************/


#include <iostream>
#include "videofilehandler.h"

VideoFileHandler::VideoFileHandler(const std::string inputFilePath)
    : BasicFileHandler(inputFilePath) {
}

VideoFileHandler::~VideoFileHandler() {
}

void VideoFileHandler::processFile(){
    std::cout << "Processing a video file: " << this->filePath << std::endl;
}
