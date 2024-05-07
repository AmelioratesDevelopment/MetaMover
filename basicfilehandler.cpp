/***********************************************************************
 * File Name: basicfilehandler.cpp
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Implementation of the BasicFileHandler class. This file
 *              includes basic operations to process generic files, primarily
 *              logging the file processing. It serves as a foundation for more
 *              specialized file handlers.
 * License: MIT License
 ***********************************************************************/


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
