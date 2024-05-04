#ifndef BASICFILEHANDLER_H
#define BASICFILEHANDLER_H

#include "IFileHandler.h"
#include <string>

class BasicFileHandler : public IF{
protected:
    std::string filePath;

public:
    BasicFileHandler(const std::string& filePath);
    virtual ~BasicFileHandler();
    virtual void processFile();
    static int getTotalBasicFiles();

private:
    static int totalBasicFiles;
};

#endif // BASICFILEHANDLER_H
