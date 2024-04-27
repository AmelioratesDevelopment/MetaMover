#include <iostream>
#include "videofilehandler.h"

VideoFileHandler::VideoFileHandler(const std::string& filePath)
    : BasicFileHandler(filePath) {}

void VideoFileHandler::processFile() {
    std::cout << "Processing a video file: " << filePath << std::endl;
    // Implement photo-specific processing here
}
