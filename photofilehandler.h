#ifndef PHOTOFILEHANDLER_H
#define PHOTOFILEHANDLER_H

#include <string>
#include "BasicFileHandler.h"

class PhotoFileHandler : public BasicFileHandler {
private:

public:
    PhotoFileHandler(const std::string& filePath);
    void processFile() override; // Must implement this method.
};
#endif // PHOTOFILEHANDLER_H
