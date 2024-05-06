#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H


class IFileHandler{
public:
    virtual void processFile() = 0;
    virtual ~IFileHandler() = default;
};

#endif // IFILEHANDLER_H
