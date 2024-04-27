#ifndef BASICFILEHANDLER_H
#define BASICFILEHANDLER_H

#include <string>

class BasicFileHandler {
protected:
    std::string filePath;

public:
    BasicFileHandler(const std::string& filePath);
    virtual ~BasicFileHandler();

    virtual void processFile();
};

#endif // BASICFILEHANDLER_H
