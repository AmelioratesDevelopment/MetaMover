#ifndef TRANSFERMANAGER_H
#define TRANSFERMANAGER_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QDir>
#include <QString>
#include <vector>
#include <memory>
#include "photofilehandler.h"
#include "directorytransfer.h"
#include "appconfigmanager.h"

class TransferManager : public QObject {
    Q_OBJECT

public:
    explicit TransferManager(QObject* parent = nullptr);
    ~TransferManager();
    void processPhotoFiles(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                           std::vector<std::unique_ptr<PhotoFileHandler>> &invalidPhotoFileHandlers);
    int const getTransferProgress();
    bool moveFiles;

signals:
    void transferProgress(int progress);
    void finished();

private slots:
    void emitTransferProgress();
    void processFiles();

private:
    void processDuplicatePhotoFiles();
    void addDuplicateTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers);
    void createDirectoryIfNotExists(const std::string& path);
    void addDirectoryTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                               std::string outputDirectory = "");
    std::string createNumericalFileName(const std::string& fileName,
                                        const std::string &targetDirectory,
                                        bool forceCopySuffix = false);
    std::string generateDirectoryPath(PhotoFileHandler* handler);
    QTimer* progressTimer;
    int progressCounter;
    std::map<std::string, DirectoryTransfer> directoryTransferMap;
    std::map<std::string, DirectoryTransfer> duplicatesTransferMap;
    std::vector<DirectoryTransfer> photoTransfers;
    DirectoryTransfer invalidPhotoTransfers;
    DirectoryTransfer DuplicatePhotoTransfers;
    AppConfigManager configManager;
};

#endif // TRANSFERMANAGER_H
