#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <vector>
#include "photofilehandler.h"
#include "exif.h"

PhotoFileHandler::PhotoFileHandler(const std::string inputFilePath)
    : BasicFileHandler(inputFilePath) {
    fileValid = false;
    containsEXIFData = false;
    validCreationDataInEXIF = false;
    hasEXIFDateWODate = false;
}

PhotoFileHandler::~PhotoFileHandler() {
}

void PhotoFileHandler::processFile() {
    std::cout << "Processing a photo file: " << filePath << std::endl;
    extractEXIFData();
}

void PhotoFileHandler::extractEXIFData(){
    // Open the file with std::ifstream in binary mode
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Can't open file.\n";
        fileValid = false;
        return;
    }

    // Read the data into a std::vector
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});

    // Ensure the size does not exceed the maximum for unsigned int
    if (buffer.size() > std::numeric_limits<unsigned int>::max()) {
        std::cerr << "File size too large for processing.\n";
        fileValid = false;
        return;
    }
    fileValid = true;

    // Parse EXIF
    // Safe cast because we checked the size against unsigned int limits
    unsigned int size = static_cast<unsigned int>(buffer.size());
    int code = exifData.parseFrom(buffer.data(), size);
    if (code) {
        std::cerr << "Error parsing EXIF: code " << code << "\n";
        containsEXIFData = false;
        return;
    }
    containsEXIFData = true;
    /**
    printf("Camera make          : %s\n", exifData.Make.c_str());
    printf("Camera model         : %s\n", exifData.Model.c_str());
    printf("Software             : %s\n", exifData.Software.c_str());
    printf("Bits per sample      : %d\n", exifData.BitsPerSample);
    printf("Image width          : %d\n", exifData.ImageWidth);
    printf("Image height         : %d\n", exifData.ImageHeight);
    printf("Image description    : %s\n", exifData.ImageDescription.c_str());
    printf("Image orientation    : %d\n", exifData.Orientation);
    printf("Image copyright      : %s\n", exifData.Copyright.c_str());
    printf("Image date/time      : %s\n", exifData.DateTime.c_str());
    printf("Original date/time   : %s\n", exifData.DateTimeOriginal.c_str());
    printf("Digitize date/time   : %s\n", exifData.DateTimeDigitized.c_str());
    printf("Subsecond time       : %s\n", exifData.SubSecTimeOriginal.c_str());
    printf("Exposure time        : 1/%d s\n",
           (unsigned)(1.0 / exifData.ExposureTime));
    printf("F-stop               : f/%.1f\n", exifData.FNumber);
    printf("Exposure program     : %d\n", exifData.ExposureProgram);
    printf("ISO speed            : %d\n", exifData.ISOSpeedRatings);
    printf("Subject distance     : %f m\n", exifData.SubjectDistance);
    printf("Exposure bias        : %f EV\n", exifData.ExposureBiasValue);
    printf("Flash used?          : %d\n", exifData.Flash);
    printf("Flash returned light : %d\n", exifData.FlashReturnedLight);
    printf("Flash mode           : %d\n", exifData.FlashMode);
    printf("Metering mode        : %d\n", exifData.MeteringMode);
    printf("Lens focal length    : %f mm\n", exifData.FocalLength);
    printf("35mm focal length    : %u mm\n", exifData.FocalLengthIn35mm);
    printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
           exifData.GeoLocation.Latitude, exifData.GeoLocation.LatComponents.degrees,
           exifData.GeoLocation.LatComponents.minutes,
           exifData.GeoLocation.LatComponents.seconds,
           exifData.GeoLocation.LatComponents.direction);
    printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
           exifData.GeoLocation.Longitude, exifData.GeoLocation.LonComponents.degrees,
           exifData.GeoLocation.LonComponents.minutes,
           exifData.GeoLocation.LonComponents.seconds,
           exifData.GeoLocation.LonComponents.direction);
    printf("GPS Altitude         : %f m\n", exifData.GeoLocation.Altitude);
    printf("GPS Precision (DOP)  : %f\n", exifData.GeoLocation.DOP);
    printf("Lens min focal length: %f mm\n", exifData.LensInfo.FocalLengthMin);
    printf("Lens max focal length: %f mm\n", exifData.LensInfo.FocalLengthMax);
    printf("Lens f-stop min      : f/%.1f\n", exifData.LensInfo.FStopMin);
    printf("Lens f-stop max      : f/%.1f\n", exifData.LensInfo.FStopMax);
    printf("Lens make            : %s\n", exifData.LensInfo.Make.c_str());
    printf("Lens model           : %s\n", exifData.LensInfo.Model.c_str());
    printf("Focal plane XRes     : %f\n", exifData.LensInfo.FocalPlaneXResolution);
    printf("Focal plane YRes     : %f\n", exifData.LensInfo.FocalPlaneYResolution);
    **/
}
