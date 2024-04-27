#ifndef VIDEOFILEHANDLER_H
#define VIDEOFILEHANDLER_H

#include <string>
#include "basicfilehandler.h"

class VideoFileHandler : public BasicFileHandler {
    private:

    public:
        VideoFileHandler(const std::string& filePath);
        void processFile() override; // Must implement this method.
};
#endif // VIDEOFILEHANDLER_H
