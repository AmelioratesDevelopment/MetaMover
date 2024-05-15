#ifndef DIRECTORYTRANSFER_H
#define DIRECTORYTRANSFER_H

#include <vector>
#include <memory>
#include "photofilehandler.h"

class DirectoryTransfer
{
public:
    DirectoryTransfer() = default;
    DirectoryTransfer(std::string inputTargetDirectory);
    void setPhotoFilesToTransfer(std::vector<std::unique_ptr<PhotoFileHandler>> &inputPhotoFiles);
    void setTargetDirectory(std::string targetDirectory);
    void addPhotoFileToTransfer(std::unique_ptr<PhotoFileHandler> &photoFile);
    bool transferFiles(bool move = false);
    bool checkFilenameMatch(const std::string& targetFilename);    
    bool removePhotoFileFromTransfer(const std::unique_ptr<PhotoFileHandler>& photoFile);
    bool movePhotoFileToAnotherVector(const std::unique_ptr<PhotoFileHandler>& photoFile,
                                                         std::vector<std::unique_ptr<PhotoFileHandler>>& targetVector);
    std::vector<std::unique_ptr<PhotoFileHandler>> getAllPhotoFilenameDuplicates();
    std::vector<std::unique_ptr<PhotoFileHandler>> getAllPhotoEXIFDuplicates();
    std::vector<std::unique_ptr<PhotoFileHandler>>& getPhotoFileToTransfer();
    void createDirectoryIfNotExists(const std::string& path);
    void clear();
    int getFilesToMoveCount();
private:
    std::vector<std::unique_ptr<PhotoFileHandler>> photoFilesToTransfer;
    std::string targetDirectory;
};

#endif // DIRECTORYTRANSFER_H
