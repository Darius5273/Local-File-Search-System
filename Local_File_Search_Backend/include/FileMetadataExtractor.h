#ifndef FILE_METADATA_EXTRACTOR_H
#define FILE_METADATA_EXTRACTOR_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <sstream>
#include <iomanip>
#include <magic.h>
#include "FileData.h"
#include "IndexScorer.h"

namespace fs = std::filesystem;

class FileMetadataExtractor {
private:
    const char* MAGIC_DATABASE_FILE_PATH = "D:/vcpkg/installed/x64-windows/share/libmagic/misc/magic.mgc";
    magic_t magicCookie;
    IndexScorer indexScorer;
public:
    FileMetadataExtractor();

    ~FileMetadataExtractor();

    FileData extractMetadata(const fs::directory_entry &entry);

    std::string getMimeType(const std::string &extension);

    std::string getFormattedTime(const fs::file_time_type &ftime);

};
#endif // FILE_METADATA_EXTRACTOR_H
