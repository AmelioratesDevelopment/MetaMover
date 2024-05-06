#ifndef BASICFILEHANDLER_H
#define BASICFILEHANDLER_H

#include "ifilehandler.h"
#include <string>


class BasicFileHandler : public IFileHandler {
protected:
    std::string filePath;


public:
    BasicFileHandler(const std::string inputFilePath);
    virtual ~BasicFileHandler();
    virtual void processFile() override;

private:
    BasicFileHandler() = delete;
};

#endif // BASICFILEHANDLER_H
