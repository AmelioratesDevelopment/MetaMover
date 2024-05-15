#ifndef TRANSFERMANAGER_H
#define TRANSFERMANAGER_H

#include <vector>
#include <memory>
#include <QObject>
#include "photofilehandler.h"
#include "directorytransfer.h"
#include "appconfigmanager.h"

class TransferManager : public QObject {
    Q_OBJECT

public:
    explicit TransferManager(QObject* parent = nullptr);
    void processPhotoFiles(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                           std::vector<std::unique_ptr<PhotoFileHandler>> &invalidPhotoFileHandlers,
                           bool moveFiles = false);
    void processDuplicatePhotoFiles();
    void findDuplicatePhotoFiles(std::vector<DirectoryTransfer> photoFileHandlers);
    void addDirectoryTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                              std::string outputDirectory = "");
    void addDuplicateTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers);
    void createDirectoryIfNotExists(const std::string& path);
    std::string createNumericalFileName(const std::string& fileName,
                                        const std::string &targetDirectory,
                                        bool forceCopySuffix = false);
    bool prepareTransfer();
    bool startTransfers();
private:
    std::map<std::string, DirectoryTransfer> directoryTransferMap;
    std::map<std::string, DirectoryTransfer> duplicatesTransferMap;
    std::vector<DirectoryTransfer> photoTransfers;
    std::string generateDirectoryPath(PhotoFileHandler* handler);
    DirectoryTransfer invalidPhotoTransfers;
    DirectoryTransfer DuplicatePhotoTransfers;
    AppConfigManager configManager;
};

#endif // TRANSFERMANAGER_H
