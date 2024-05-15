#include <filesystem>
#include <iostream>
#include "directorytransfer.h"
#include "scanner.h"

DirectoryTransfer::DirectoryTransfer(const std::string inputTargetDirectory)
    : targetDirectory(inputTargetDirectory){
}

void DirectoryTransfer::setTargetDirectory(std::string targetDirectory){
    this->targetDirectory = targetDirectory;
}

void DirectoryTransfer::setPhotoFilesToTransfer(std::vector<std::unique_ptr<PhotoFileHandler>> &inputPhotoFiles){
    for (auto& handler : inputPhotoFiles){
        addPhotoFileToTransfer(handler);
    }
}

void DirectoryTransfer::addPhotoFileToTransfer(std::unique_ptr<PhotoFileHandler> &photoFile){
    this->photoFilesToTransfer.push_back(std::move(photoFile));
}

bool DirectoryTransfer::removePhotoFileFromTransfer(const std::unique_ptr<PhotoFileHandler>& photoFile) {
    // Ensure the photoFile has not been moved or invalidated elsewhere in your application.
    if (!photoFile) {
        return false; // Indicates that the passed unique_ptr is empty or invalid.
    }

    std::string targetPath = photoFile->getSourceFilePath();  // Get the file path from the object
    auto it = std::remove_if(photoFilesToTransfer.begin(), photoFilesToTransfer.end(),
                             [&targetPath](const std::unique_ptr<PhotoFileHandler>& item) {
                                 return item->getSourceFilePath() == targetPath;
                             });
    bool removed = (it != photoFilesToTransfer.end());
    photoFilesToTransfer.erase(it, photoFilesToTransfer.end());
    return removed; // Return true if any elements were removed.
}

bool DirectoryTransfer::movePhotoFileToAnotherVector(const std::unique_ptr<PhotoFileHandler>& photoFile,
                                                     std::vector<std::unique_ptr<PhotoFileHandler>>& targetVector) {
    // Ensure the photoFile has not been moved or invalidated elsewhere in your application.
    if (!photoFile) {
        return false; // Indicates that the passed unique_ptr is empty or invalid.
    }

    std::string targetPath = photoFile->getSourceFilePath();  // Get the file path from the object
    auto it = std::find_if(photoFilesToTransfer.begin(), photoFilesToTransfer.end(),
                           [&targetPath](const std::unique_ptr<PhotoFileHandler>& item) {
                               return item->getSourceFilePath() == targetPath;
                           });
    if (it != photoFilesToTransfer.end()) {
        targetVector.push_back(std::move(*it));  // Move the unique_ptr to the target vector
        photoFilesToTransfer.erase(it);          // Erase the moved-from pointer from the original vector
        return true; // Indicates success.
    }
    return false; // Indicates that no element was found to move.
}

bool DirectoryTransfer::transferFiles(bool move){
    // ensure target directory exists
    createDirectoryIfNotExists(targetDirectory);
    // commence copy or move of all files in the list:
    for (const auto& photoHandler : photoFilesToTransfer) {
        // Construct the source and target paths
        std::filesystem::path sourcePath(photoHandler->getSourceFilePath());
        std::filesystem::path targetPath = std::filesystem::path(targetDirectory) / photoHandler->getTargetFileName();
        try {
            if (move) {
                if (photoHandler->overwriteEnabled || !std::filesystem::exists(targetPath)) {
                    std::filesystem::rename(sourcePath, targetPath); // Move file
                    std::cout << "Moved file: " << sourcePath << " to " << targetPath << std::endl; // Debug log
                } else {
                    std::cerr << "File already exists and overwrite is disabled: " << targetPath << std::endl;
                }
            } else {
                if (photoHandler->overwriteEnabled) {
                    std::filesystem::copy(sourcePath, targetPath, std::filesystem::copy_options::overwrite_existing); // Copy file with overwrite
                } else {
                    std::filesystem::copy(sourcePath, targetPath, std::filesystem::copy_options::skip_existing); // Copy file without overwrite
                }
                std::cout << "Copied file: " << sourcePath << " to " << targetPath << std::endl; // Debug log
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl; // Log error
            return false;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl; // Log error
            return false;
        }
    }
    return true;
}

std::vector<std::unique_ptr<PhotoFileHandler>> DirectoryTransfer::getAllPhotoFilenameDuplicates(){
    std::vector<std::unique_ptr<PhotoFileHandler>> duplicatesFound;
    // Use an iterator to allow safe erasing while iterating
    for (auto it = photoFilesToTransfer.begin(); it != photoFilesToTransfer.end(); ) {
        if (*it && checkFilenameMatch((*it)->getTargetFileName())) {
            duplicatesFound.push_back(std::move(*it)); // Move the unique_ptr to duplicatesFound
            it = photoFilesToTransfer.erase(it); // Erase returns the next valid iterator
        } else {
            ++it; // Only increment if not erasing
        }
    }
    return duplicatesFound;
}

std::vector<std::unique_ptr<PhotoFileHandler>> DirectoryTransfer::getAllPhotoEXIFDuplicates() {
    std::vector<std::unique_ptr<PhotoFileHandler>> duplicatesFound;
    std::vector<std::unique_ptr<PhotoFileHandler>> uniquePhotos;

    // check internally against the source files for any matching files
    // (files can be named differently and still match)
    for (auto it1 = photoFilesToTransfer.begin(); it1 != photoFilesToTransfer.end(); ) {
        bool duplicateFound = false;

        for (auto it2 = uniquePhotos.begin(); it2 != uniquePhotos.end(); ++it2) {
            if ((*it1)->getExifData() == (*it2)->getExifData()) {
                // Compare creation times to determine which to keep as duplicate
                if ((*it1)->getFileCreationTime() < (*it2)->getFileCreationTime()) {
                    // it1 is older, move it to duplicatesFound
                    duplicatesFound.push_back(std::move(*it1));
                    it1 = photoFilesToTransfer.erase(it1); // Erase returns the next valid iterator
                } else {
                    // it2 is older, move it to duplicatesFound
                    duplicatesFound.push_back(std::move(*it2));
                    *it2 = std::move(*it1); // Move it1 to the position of it2
                    it1 = photoFilesToTransfer.erase(it1); // Erase it1
                }
                duplicateFound = true;
                break; // Break out of the inner loop
            }
        }

        if (!duplicateFound) {
            // Move the unique photo to uniquePhotos
            uniquePhotos.push_back(std::move(*it1));
            it1 = photoFilesToTransfer.erase(it1); // Erase returns the next valid iterator
        }
    }

    // Move unique photos back to photoFilesToTransfer
    for (auto& uniquePhoto : uniquePhotos) {
        photoFilesToTransfer.push_back(std::move(uniquePhoto));
    }
    uniquePhotos.clear();

    // Now check the target directory for possible matches (if it exists)
    if (std::filesystem::exists(targetDirectory)) {
        Scanner targetDirectoryScanner;
        targetDirectoryScanner.scan(targetDirectory, false);
        std::vector<std::unique_ptr<PhotoFileHandler>> &targetDirectoryPhotoFileHandlers = targetDirectoryScanner.getPhotoFileHandlers();
        // Use an iterator to allow safe erasing while iterating
        for (auto it1 = photoFilesToTransfer.begin(); it1 != photoFilesToTransfer.end(); ) {
            bool duplicateFound = false;
            for (auto it2 = targetDirectoryPhotoFileHandlers.begin(); it2 != targetDirectoryPhotoFileHandlers.end(); ++it2) {
                if (*it1 && *it2 && (*it1)->getExifData() == (*it2)->getExifData()) {
                    duplicatesFound.push_back(std::move(*it1)); // Move the unique_ptr to duplicatesFound
                    it1 = photoFilesToTransfer.erase(it1); // Erase returns the next valid iterator
                    duplicateFound = true;
                    break; // Break out of the inner loop once a duplicate is found and handled
                }
            }
            if (!duplicateFound) {
                ++it1; // Only increment if no duplicate was found
            }
        }
    }
    return duplicatesFound;
}

std::vector<std::unique_ptr<PhotoFileHandler>>& DirectoryTransfer::getPhotoFileToTransfer()
{
    return photoFilesToTransfer;
}

int DirectoryTransfer::getFilesToMoveCount(){
    int filesToMove = static_cast<int>(photoFilesToTransfer.size());
    return filesToMove;
}


bool DirectoryTransfer::checkFilenameMatch(const std::string& targetFilename) {
    std::filesystem::path path{targetDirectory};
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cerr << "Provided path is not a valid directory." << std::endl;
        return false; //Path doesn't exist - so there should not a copy or move conflict
    }

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().filename() == targetFilename) {
            return true;  // Filename match found
        }
    }

    return false;  // No match found
}

void DirectoryTransfer::createDirectoryIfNotExists(const std::string& path) {
    std::filesystem::create_directories(path);
}

void DirectoryTransfer::clear(){
    photoFilesToTransfer.clear();
    targetDirectory = "";
}
