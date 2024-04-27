#include <iostream>
#include "basicfilehandler.h"

BasicFileHandler::BasicFileHandler(const std::string& filePath)
    : filePath(filePath) {}

BasicFileHandler::~BasicFileHandler() {}

void BasicFileHandler::processFile() {
    std::cout << "Processing a photo file: " << filePath << std::endl;
    // Implement photo-specific processing here
}
