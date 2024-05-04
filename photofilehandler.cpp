#include <iostream>
#include "photofilehandler.h"

int PhotoFileHandler::totalPhotoFiles {0};

PhotoFileHandler::PhotoFileHandler(const std::string& filePath)
    : BasicFileHandler(filePath) {}

void PhotoFileHandler::processFile() {
    std::cout << "Processing a photo file: " << filePath << std::endl;
    // Implement photo-specific processing here
}
