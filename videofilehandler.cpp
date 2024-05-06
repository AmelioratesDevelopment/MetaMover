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
