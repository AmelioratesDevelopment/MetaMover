#ifndef TRANSFERMANAGER_H
#define TRANSFERMANAGER_H

/***********************************************************************
 * File Name: transfermanager.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-19
 * Description: Header file for the TransferManager class, which handles
 *              the management and transfer of photo files. This
 *              class leverages modern C++ techniques, including smart pointers
 *              and atomic operations for thread-safe management of transfer
 *              processes. The class integrates with the AppConfigManager to
 *              dynamically adjust configurations and supports signal-slot
 *              mechanisms for asynchronous operations in Qt.
 * License: MIT License
 ***********************************************************************/


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

    int const getTransferProgress();
    void resetTransferManager();
    std::atomic<bool> transferRunning{false};
    std::atomic<bool> cancelTransfer{false};

signals:
    void transferComplete();

public slots:
    void processPhotoFiles(std::vector<std::unique_ptr<PhotoFileHandler>> *photoFileHandlers,
                           std::vector<std::unique_ptr<PhotoFileHandler>> *invalidPhotoFileHandlers,
                           bool moveFiles = false);

private:
    void processDuplicatePhotoFiles();
    void processFileTransfers(bool moveFiles = false);
    void addDuplicateTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers);
    void createDirectoryIfNotExists(const std::string& path);
    void addDirectoryTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                               std::string outputDirectory = "");
    std::string createNumericalFileName(const std::string& fileName,
                                        const std::string &targetDirectory,
                                        bool forceCopySuffix = false);
    std::string generateDirectoryPath(PhotoFileHandler* handler);
    std::string getMonthName(int monthNumber);
    QTimer* progressTimer;
    std::atomic<int> progressCounter{0};
    std::map<std::string, DirectoryTransfer> directoryTransferMap;
    std::map<std::string, DirectoryTransfer> duplicatesTransferMap;
    std::vector<DirectoryTransfer> photoTransfers;
    DirectoryTransfer invalidPhotoTransfers;
    DirectoryTransfer DuplicatePhotoTransfers;
    AppConfigManager configManager;
};

#endif // TRANSFERMANAGER_H
