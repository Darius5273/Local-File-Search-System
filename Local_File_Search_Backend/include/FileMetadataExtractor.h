#ifndef FILE_METADATA_EXTRACTOR_H
#define FILE_METADATA_EXTRACTOR_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <sstream>
#include <iomanip>
#include "FileData.h"

namespace fs = std::filesystem;

class FileMetadataExtractor {
public:
    FileData extractMetadata(const fs::directory_entry &entry);

    std::string getMimeType(const std::string &extension);

    std::string getFormattedTime(const fs::file_time_type &ftime);

};
#endif // FILE_METADATA_EXTRACTOR_H
