#ifndef VIDEOFILEHANDLER_H
#define VIDEOFILEHANDLER_H

#include <string>
#include "basicfilehandler.h"

class VideoFileHandler : public BasicFileHandler {
protected:

public:
    VideoFileHandler(const std::string inputFilePath);
    virtual ~VideoFileHandler();
    virtual void processFile() override;

private:
    VideoFileHandler() = delete;
};
#endif // VIDEOFILEHANDLER_H
