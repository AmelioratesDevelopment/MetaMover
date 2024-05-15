
#include <ctime>
#include <sstream>
#include <iostream>
#include <regex>
#include <set>
#include <QDir>
#include <QString>
#include "transfermanager.h"

TransferManager::TransferManager(QObject* parent)
    : QObject(parent), configManager(AppConfig::get()){}


void TransferManager::processPhotoFiles(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                                        std::vector<std::unique_ptr<PhotoFileHandler>> &invalidPhotoFileHandlers,
                                        bool moveFiles){
    addDirectoryTransfers(photoFileHandlers);
    //Indentify Duplicates and Update Directory Transfer Map
    processDuplicatePhotoFiles();
    // If enabled add invaild transfers to the directoryTransferMap
    if(configManager.config.getMoveInvalidFileMeta()){
        addDirectoryTransfers(invalidPhotoFileHandlers, configManager.config.getInvalidFileMetaDirectory());
    }
    for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end(); ++dt){
        dt->second.transferFiles(moveFiles);
    }
    //cleanup
    directoryTransferMap.clear();
    duplicatesTransferMap.clear();
    photoTransfers.clear();
    invalidPhotoTransfers.clear();
    DuplicatePhotoTransfers.clear();
}

void TransferManager::processDuplicatePhotoFiles(){
    try {
        // Identify Duplicates
        std::vector<std::unique_ptr<PhotoFileHandler>> duplicatesList;
        if(configManager.config.getPhotosDuplicateIdentitySetting() == "File Names Match")
        {
            //Check the potential copy operation for same filename in the target directory
            for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end(); ++dt){
                std::vector<std::unique_ptr<PhotoFileHandler>> tempDuplicates = dt->second.getAllPhotoFilenameDuplicates();
                for (auto& duplicate : tempDuplicates) {
                    duplicatesList.push_back(std::move(duplicate));
                }
            }
        }
        if(configManager.config.getPhotosDuplicateIdentitySetting() == "All EXIF and Exact File Contents Match")
        {
            //Check the potential copy operation for same filename in the target directory
            for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end(); ++dt){
                std::string key = (*dt).first;
                std::vector<std::unique_ptr<PhotoFileHandler>> tempDuplicates = directoryTransferMap[(*dt).first].getAllPhotoEXIFDuplicates();
                for (auto& duplicate : tempDuplicates) {
                    duplicatesList.push_back(std::move(duplicate));
                }
            }
        }
        addDuplicateTransfers(duplicatesList);
        // removing duplicates can cause empty entries in the transfer map - clean up
        for(auto dt = directoryTransferMap.begin(); dt != directoryTransferMap.end();){
            if(dt->second.getFilesToMoveCount() == 0){
                dt = directoryTransferMap.erase(dt);
            } else {
                ++dt;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in processDuplicatePhotoFiles: " << e.what() << std::endl;
        throw; // Re-throw the exception after logging it
    } catch (...) {
        std::cerr << "Unknown exception caught in processDuplicatePhotoFiles" << std::endl;
        throw; // Re-throw the unknown exception after logging it
    }

}

void TransferManager::addDirectoryTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers,
                          std::string outputDirectory)
{
    if(outputDirectory != ""){
        directoryTransferMap[outputDirectory].setPhotoFilesToTransfer(photoFileHandlers);
        directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
        return;
    }
    // outputDirectory not specified - derive the path from the folder structure selection
    for (auto& handler : photoFileHandlers) {
        outputDirectory = generateDirectoryPath(handler.get());
        directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
        directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
    }
}

void TransferManager::addDuplicateTransfers(std::vector<std::unique_ptr<PhotoFileHandler>> &photoFileHandlers)
{
    try {
        std::string selection = configManager.config.getDuplicatesFoundSelection();
        if(selection == "Add 'Copy##' and Move/Copy"){
            for(auto& handler : photoFileHandlers){
                std::string outputDirectory = generateDirectoryPath(handler.get());
                handler->setTargetFileName(createNumericalFileName(handler->getTargetFileName(),
                                                                   outputDirectory));
                directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
                directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
            }
        }else if(selection == "Do Not Move or Copy"){
            photoFileHandlers.clear();
        }else if(selection == "Overwrite"){
            for(auto& handler : photoFileHandlers){
                std::string outputDirectory;
                handler->overwriteEnabled = true;
                outputDirectory = generateDirectoryPath(handler.get());
                directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
                directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
            }
        }else if(selection == "Move To Folder"){
            for(auto& handler : photoFileHandlers){
                std::string outputDirectory = configManager.config.getDuplicatesDirectory();
                handler->setTargetFileName(createNumericalFileName(handler->getTargetFileName(),
                                                                   outputDirectory));
                directoryTransferMap[outputDirectory].addPhotoFileToTransfer(handler);
                directoryTransferMap[outputDirectory].setTargetDirectory(outputDirectory);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in addDuplicateTransfers: " << e.what() << std::endl;
        throw; // Re-throw the exception after logging it
    } catch (...) {
        std::cerr << "Unknown exception caught in addDuplicateTransfers" << std::endl;
        throw; // Re-throw the unknown exception after logging it
    }
}

std::string TransferManager::createNumericalFileName(const std::string& fileName,
                                                     const std::string& targetDirectory,
                                                     bool forceCopySuffix) {
    std::filesystem::path dirPath(targetDirectory);
    std::filesystem::path filePath(fileName);

    std::string baseFilename = filePath.stem().string(); // Extract the base filename without extension
    std::string extension = filePath.extension().string(); // Extract the extension

    // Check if the base filename already exists in the directory
    std::filesystem::path baseFilePath = dirPath / (baseFilename + extension);
    if (!std::filesystem::exists(baseFilePath) && !forceCopySuffix) {
        return fileName; // Return the original filename if no conflict exists
    }

    // Regex to find and isolate numeric suffixes in the format 'filename_Copy##'
    std::regex pattern("(_Copy\\d+)$");
    std::smatch matches;

    if (std::regex_search(baseFilename, matches, pattern)) {
        baseFilename = baseFilename.substr(0, matches.position(0)); // Remove the numeric suffix from base filename
    }

    std::set<int> existingNumbers;
    std::regex fullPattern(baseFilename + "_Copy(\\d+)" + std::regex_replace(extension, std::regex("\\."), "\\."));

    // Iterate over files in the directory
    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        std::string filename = entry.path().filename().string();
        if (std::regex_match(filename, matches, fullPattern) && matches.size() == 2) {
            existingNumbers.insert(std::stoi(matches[1].str())); // Add found number to the set
        }
    }

    //Iterate over the respective photoFileToTransfer from the directoryTransferMap
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

    // Find the smallest missing number in the sequence
    int newNumber = 0;
    while (existingNumbers.count(newNumber)) {
        ++newNumber;
    }

    // Create the new filename with the appropriate 'Copy' and number
    std::ostringstream oss;
    oss << baseFilename << "_Copy" << std::setw(2) << std::setfill('0') << newNumber << extension;
    return oss.str();
}


bool TransferManager::prepareTransfer() {
    // identify duplicates here
    return true;
}

bool TransferManager::startTransfers(){
    return true;
}

std::string TransferManager::generateDirectoryPath(PhotoFileHandler* handler) {
    auto timePoint = handler->getOriginalDateTime();
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm dateTime;

// Use localtime_s or localtime_r depending on platform
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

void TransferManager::findDuplicatePhotoFiles(std::vector<DirectoryTransfer> photoFileHandlers){

}




/*std::vector<DirectoryTransfer> photoTransfers;
DirectoryTransfer invalidPhotoTransfers;
DirectoryTransfer DuplicatePhotoTransfers;
AppConfigManager configManager;*/
