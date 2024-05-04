#ifndef PHOTOFILEHANDLER_H
#define PHOTOFILEHANDLER_H

#include <string>
#include "BasicFileHandler.h"

class PhotoFileHandler : public BasicFileHandler {
private:
    static int totalPhotoFiles;

public:
    PhotoFileHandler(const std::string& filePath);
    static int getTotalPhotoFiles();
    void processFile() override; // Must implement this method.
};
#endif // PHOTOFILEHANDLER_H
