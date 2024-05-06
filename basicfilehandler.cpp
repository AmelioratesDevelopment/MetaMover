#include <iostream>
#include "basicfilehandler.h"

BasicFileHandler::BasicFileHandler(const std::string inputFilePath)
    : filePath(inputFilePath) {
}

BasicFileHandler::~BasicFileHandler() {
}

void BasicFileHandler::processFile(){
    std::cout << "Processing a basic file: " << this->filePath << std::endl;
}
