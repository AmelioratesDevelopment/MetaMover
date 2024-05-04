#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <QFile>

class IFileHandler{
public:
    virtual void processFile(std::string filePath) = 0;
    virtual ~IFileHandler();
protected:
    QFile file;
};

#endif // IFILEHANDLER_H
