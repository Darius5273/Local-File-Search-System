#include "../include/FileMetadata.h"

FileMetadata::FileMetadata(const std::string& name, const std::string& path,
                           const std::string& extension, const std::string& mime_type,
                           const std::string& modified_time, const std::string& created_at,
                           long long size)
        : name(name), path(path), extension(extension), mime_type(mime_type),
          modified_time(modified_time), created_at(created_at), size(size) {}
