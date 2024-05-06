#ifndef FILEHANDLERFACTORY_H
#define FILEHANDLERFACTORY_H

#include <memory>
#include <map>
#include <string>
#include <filesystem>
#include "basicfilehandler.h"
#include "photofilehandler.h"
#include "videofilehandler.h"

struct FileHandlerFactory //Abstract Factory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const=0;
public:
    virtual ~FileHandlerFactory() = default;
    FileHandlerFactory() = default;
};

struct BasicFileHandlerFactory : FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const override{
        auto handler = std::make_unique<BasicFileHandler>(filePath);
        handler->processFile();
        return handler;
    }
};

struct PhotoFileHandlerFactory : FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const override{
        auto handler = std::make_unique<PhotoFileHandler>(filePath);
        handler->processFile();
        return handler;
    }
};

struct VideoFileHandlerFactory : FileHandlerFactory
{
    virtual std::unique_ptr<BasicFileHandler> make(std::string filePath) const override{
        auto handler = std::make_unique<VideoFileHandler>(filePath);
        handler->processFile();
        return handler;
    }
};

struct FileFactory
{
    std::map<std::string, std::unique_ptr<FileHandlerFactory>> file_factories;
public:
    FileFactory()
    {
        // Supported Photo File Extension Types
        file_factories["jpeg"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["jpg"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["png"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["gif"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["bmp"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["tiff"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["tif"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["svg"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["webp"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["heif"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["heic"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["raw"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["cr2"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["nef"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["orf"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["psd"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["ico"] = std::make_unique<PhotoFileHandlerFactory>();
        file_factories["exr"] = std::make_unique<PhotoFileHandlerFactory>();
        // Supported Video File Extension Types
        file_factories["avi"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["flv"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["wmv"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["mov"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["mp4"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["m4v"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["mpg"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["mpeg"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["3gp"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["mkv"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["webm"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["vob"] = std::make_unique<VideoFileHandlerFactory>();
        file_factories["ogg"] = std::make_unique<VideoFileHandlerFactory>();
    }

    std::unique_ptr<BasicFileHandler> makeFileHandler(const std::string filePath)
    {
        std::filesystem::path path(filePath);
        std::string extension = path.has_extension() ? path.extension().string().substr(1) : "";

        auto it = file_factories.find(extension);
        if (it != file_factories.end()) {
            return it->second->make(filePath);
        } else {
            return std::make_unique<BasicFileHandler>(filePath);
        }
    }
};


#endif // FILEHANDLERFACTORY_H
