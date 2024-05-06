#ifndef PHOTOFILEHANDLER_H
#define PHOTOFILEHANDLER_H

#include <string>
#include "basicfilehandler.h"
#include "exif.h"

class PhotoFileHandler : public BasicFileHandler {
protected:

public:
    PhotoFileHandler(const std::string inputFilePath);
    virtual ~PhotoFileHandler();
    virtual void processFile() override;
    bool fileValid;
    bool containsEXIFData;
    bool validCreationDataInEXIF;
    bool hasEXIFDateWODate;

private:
    void extractEXIFData();
    easyexif::EXIFInfo exifData;
    PhotoFileHandler() = delete;
};
#endif // PHOTOFILEHANDLER_H
