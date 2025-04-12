#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <string>

class FileData {
public:
    std::string id;
    std::string name;
    std::string path;
    std::string extension;
    std::string mime_type;
    std::string modified_time;
    bool is_text;
    long long size;
    double score;
    bool is_image;

    FileData(const std::string& name, const std::string& path,
                 const std::string& extension, const std::string& mime_type,
                 const std::string& modified_time, long long size, bool is_text,
                 double score, bool is_image);
};

#endif // FILE_DATA_H
