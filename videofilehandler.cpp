#include <iostream>
#include "videofilehandler.h"

int VideoFileHandler::totalVideoFiles {0};

VideoFileHandler::VideoFileHandler(const std::string& filePath)
    : BasicFileHandler(filePath) {}

void VideoFileHandler::processFile() {
    std::cout << "Processing a video file: " << filePath << std::endl;
    // Implement photo-specific processing here
}
