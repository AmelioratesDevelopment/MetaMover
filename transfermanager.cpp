#include <ctime>
#include <sstream>
#include <iostream>
#include <regex>
#include <set>
#include <filesystem>
#include "transfermanager.h"

TransferManager::TransferManager(QObject* parent)
    : QObject(parent), progressCounter(0), configManager(AppConfig::get()) {
    progressTimer = new QTimer(this);
    connect(progressTimer, &QTimer::timeout, this, &TransferManager::emitTransferProgress);
}

TransferManager::~TransferManager() {
    delete progressTimer;
}

void TransferManager::processPhotoFiles(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                                        std::vector<std::unique_ptr<PhotoFileHandler>> &invalidPhotoFileHandlers){
    addDirectoryTransfers(photoFileHandlers);
    processDuplicatePhotoFiles();

    if(configManager.config.getMoveInvalidFileMeta()){
        addDirectoryTransfers(invalidPhotoFileHandlers, configManager.config.getInvalidFileMetaDirectory());
    }

    progressCounter = 0; // Reset progress
    // Start the timer to emit progress every 100 milliseconds in the main thread
    QMetaObject::invokeMethod(progressTimer, "start", Qt::QueuedConnection, Q_ARG(int, 100));
    processFiles();
}

void TransferManager::processFiles() {
    size_t total = directoryTransferMap.size();
    size_t current = 0;

    for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end(); ++dt){
        dt->second.transferFiles(moveFiles);
        current++;
        // Calculate progress as a percentage
        progressCounter = static_cast<int>((static_cast<double>(current) / total) * 100);
    }

    // Stop the timer in the main thread once processing is done
    QMetaObject::invokeMethod(progressTimer, "stop", Qt::QueuedConnection);

    // Cleanup
    directoryTransferMap.clear();
    duplicatesTransferMap.clear();
    photoTransfers.clear();
    invalidPhotoTransfers.clear();
    DuplicatePhotoTransfers.clear();

    emit finished(); // Notify that processing is finished
}

void TransferManager::processDuplicatePhotoFiles(){
    try {
        std::vector<std::unique_ptr<PhotoFileHandler>> duplicatesList;
        if(configManager.config.getPhotosDuplicateIdentitySetting() == "File Names Match") {
            for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end(); ++dt){
                std::vector<std::unique_ptr<PhotoFileHandler>> tempDuplicates = dt->second.getAllPhotoFilenameDuplicates();
                for (auto& duplicate : tempDuplicates) {
                    duplicatesList.push_back(std::move(duplicate));
                }
            }
        }
        if(configManager.config.getPhotosDuplicateIdentitySetting() == "All EXIF and Exact File Contents Match") {
            for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end(); ++dt){
                std::vector<std::unique_ptr<PhotoFileHandler>> tempDuplicates = dt->second.getAllPhotoEXIFDuplicates();
                for (auto& duplicate : tempDuplicates) {
                    duplicatesList.push_back(std::move(duplicate));
                }
            }
        }
        addDuplicateTransfers(duplicatesList);
        for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end();){
            if(dt->second.getFilesToMoveCount() == 0){
                dt = directoryTransferMap.erase(dt);
            } else {
                ++dt;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in processDuplicatePhotoFiles: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception caught in processDuplicatePhotoFiles" << std::endl;
        throw;
    }
}

void TransferManager::addDirectoryTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                                            std::string outputDirectory) {
    if(outputDirectory != ""){
        directoryTransferMap[outputDirectory].setPhotoFilesToTransfer(photoFileHandlers);
        directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
        return;
    }
    for (auto& handler : photoFileHandlers) {
        outputDirectory = generateDirectoryPath(handler.get());
        directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
        directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
    }
}

void TransferManager::addDuplicateTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers) {
    try {
        std::string selection = configManager.config.getDuplicatesFoundSelection();
        if(selection == "Add 'Copy##' and Move/Copy") {
            for(auto& handler : photoFileHandlers){
                std::string outputDirectory = generateDirectoryPath(handler.get());
                handler->setTargetFileName(createNumericalFileName(handler->getTargetFileName(), outputDirectory));
                directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
                directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
            }
        } else if(selection == "Do Not Move or Copy") {
            photoFileHandlers.clear();
        } else if(selection == "Overwrite") {
            for(auto& handler : photoFileHandlers){
                handler->overwriteEnabled = true;
                std::string outputDirectory = generateDirectoryPath(handler.get());
                directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
                directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
            }
        } else if(selection == "Move To Folder") {
            for(auto& handler : photoFileHandlers){
                std::string outputDirectory = configManager.config.getDuplicatesDirectory();
                handler->setTargetFileName(createNumericalFileName(handler->getTargetFileName(), outputDirectory));
                directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
                directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in addDuplicateTransfers: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "Unknown exception caught in addDuplicateTransfers" << std::endl;
        throw;
    }
}

std::string TransferManager::createNumericalFileName(const std::string& fileName,
                                                     const std::string& targetDirectory,
                                                     bool forceCopySuffix) {
    std::filesystem::path dirPath(targetDirectory);
    std::filesystem::path filePath(fileName);

    std::string baseFilename = filePath.stem().string();
    std::string extension = filePath.extension().string();

    std::filesystem::path baseFilePath = dirPath / (baseFilename + extension);
    if (!std::filesystem::exists(baseFilePath) && !forceCopySuffix) {
        return fileName;
    }

    std::regex pattern("(_Copy\\d+)$");
    std::smatch matches;

    if (std::regex_search(baseFilename, matches, pattern)) {
        baseFilename = baseFilename.substr(0, matches.position(0));
    }

    std::set<int> existingNumbers;
    std::regex fullPattern(baseFilename + "_Copy(\\d+)" + std::regex_replace(extension, std::regex("\\."), "\\."));

    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        std::string filename = entry.path().filename().string();
        if (std::regex_match(filename, matches, fullPattern) && matches.size() == 2) {
            existingNumbers.insert(std::stoi(matches[1].str()));
        }
    }

    std::vector<std::unique_ptr<PhotoFileHandler>> &photoFilesToTransfer = directoryTransferMap[targetDirectory].getPhotoFileToTransfer();
    for(const auto& dt : photoFilesToTransfer){
        std::string queuedFileName = dt->getTargetFileName();
        std::filesystem::path queuedFilePath(queuedFileName);

        std::string queuedBaseFilename = queuedFilePath.stem().string();
        std::string queuedExtension = queuedFilePath.extension().string();

        if (queuedExtension == extension && std::regex_match(queuedBaseFilename, matches, fullPattern) && matches.size() == 2) {
            existingNumbers.insert(std::stoi(matches[1].str()));
        }
    }

    int newNumber = 0;
    while (existingNumbers.count(newNumber)) {
        ++newNumber;
    }

    std::ostringstream oss;
    oss << baseFilename << "_Copy" << std::setw(2) << std::setfill('0') << newNumber << extension;
    return oss.str();
}

std::string TransferManager::generateDirectoryPath(PhotoFileHandler* handler) {
    auto timePoint = handler->getOriginalDateTime();
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm dateTime;

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&dateTime, &time);
#else
    localtime_r(&time, &dateTime);
#endif

    std::vector<std::string> tokens;
    std::istringstream iss(configManager.config.getPhotosOutputFolderStructureSelection());
    std::string token;
    std::string path = configManager.config.getOutputDirectory() + "/";

    while (getline(iss, token, ',')) {
        std::string trimmed;
        std::remove_copy(token.begin(), token.end(), std::back_inserter(trimmed), ' ');
        tokens.push_back(trimmed);
    }

    for (const auto& component : tokens) {
        if (component == "Camera Model") {
            path += handler->getCameraModel() + "/";
        } else if (component == "Year") {
            path += std::to_string(dateTime.tm_year + 1900) + "/";
        } else if (component == "Month") {
            path += std::to_string(dateTime.tm_mon + 1) + "/";
        } else if (component == "Day") {
            path += std::to_string(dateTime.tm_mday) + "/";
        }
    }
    return QString(QDir::toNativeSeparators(QString::fromStdString(path))).toStdString();
}

int const TransferManager::getTransferProgress() {
    return progressCounter;
}

void TransferManager::emitTransferProgress() {
    emit transferProgress(progressCounter);
}
