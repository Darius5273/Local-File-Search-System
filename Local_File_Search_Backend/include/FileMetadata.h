#ifndef FILE_METADATA_H
#define FILE_METADATA_H

#include <string>

class FileMetadata {
public:
    std::string name;
    std::string path;
    std::string extension;
    std::string mime_type;
    std::string modified_time;
    std::string created_at;
    long long size;

    FileMetadata(const std::string& name, const std::string& path,
                 const std::string& extension, const std::string& mime_type,
                 const std::string& modified_time, const std::string& created_at,
                 long long size);
};

#endif // FILE_METADATA_H
