#ifndef FILEHANDLERFACTORY_H
#define FILEHANDLERFACTORY_H

/***********************************************************************
 * File Name: filehandlerfactory.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the FileHandlerFactory structure and its
 *              specific implementations for different file types. This abstract
 *              factory pattern is used to create appropriate file handler
 *              instances for various file types, such as basic files, photos,
 *              and videos. It also includes the FileFactory class that manages
 *              a map of file type extensions to their corresponding factories,
 *              facilitating dynamic file handler creation based on file extension.
 * License: MIT License
 ***********************************************************************/

#include <memory>
#include <map>
#include <string>
#include <filesystem>
#include "basicfilehandler.h"
#include "photofilehandler.h"
#include "videofilehandler.h"

// Abstract factory for creating file handlers
struct FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const = 0;

    virtual ~FileHandlerFactory() = default;
    FileHandlerFactory() = default;
};

// Factory for creating basic file handlers
struct BasicFileHandlerFactory : FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const override {
        auto handler = std::make_unique<BasicFileHandler>(filePath);
        handler->processFile();  // Process file immediately upon creation
        return handler;
    }
};

// Factory for creating photo file handlers
struct PhotoFileHandlerFactory : FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const override {
        auto handler = std::make_unique<PhotoFileHandler>(filePath);
        handler->processFile();  // Process photo file immediately upon creation
        return handler;
    }
};

// Factory for creating video file handlers
struct VideoFileHandlerFactory : FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const override {
        auto handler = std::make_unique<VideoFileHandler>(filePath);
        handler->processFile();  // Process video file immediately upon creation
        return handler;
    }
};

// Manages mapping of file extensions to specific file handler factories
struct FileFactory
{
    std::map<std::string, std::unique_ptr<FileHandlerFactory>> file_factories;

    FileFactory() {
        initializeFileFactories();
    }

    // Initializes the map with specific factories for different file extensions
    void initializeFileFactories() {
        // Mapping file extensions to their respective factories for photo files
        const std::vector<std::string> photoExtensions = {
            "jpeg", "jpg", "png", "gif", "bmp", "tiff", "tif", "svg", "webp",
            "heif", "heic", "raw", "cr2", "nef", "orf", "psd", "ico", "exr"
        };

        for (const auto& ext : photoExtensions) {
            file_factories[ext] = std::make_unique<PhotoFileHandlerFactory>();
        }

        // Mapping file extensions to their respective factories for video files
        const std::vector<std::string> videoExtensions = {
            "avi", "flv", "wmv", "mov", "mp4", "m4v", "mpg", "mpeg",
            "3gp", "mkv", "webm", "vob", "ogg"
        };

        for (const auto& ext : videoExtensions) {
            file_factories[ext] = std::make_unique<VideoFileHandlerFactory>();
        }
    }

    // Creates a file handler based on the file's extension
    std::unique_ptr<BasicFileHandler> makeFileHandler(const std::string filePath) {
        std::filesystem::path path(filePath);
        std::string extension = path.has_extension() ? path.extension().string().substr(1) : "";

        auto it = file_factories.find(extension);
        if (it != file_factories.end()) {
            return it->second->make(filePath);
        } else {
            return std::make_unique<BasicFileHandler>(filePath);  // Default to basic file handler
        }
    }
};

#endif // FILEHANDLERFACTORY_H
