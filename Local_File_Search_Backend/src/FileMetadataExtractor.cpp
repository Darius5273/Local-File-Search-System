
#include "../include/FileMetadataExtractor.h"
#include "../include/Utf8Converter.h"
#include <magic.h>

FileMetadataExtractor::FileMetadataExtractor() {
    magicCookie = magic_open(MAGIC_MIME_TYPE);
    if (!magicCookie) {
        throw std::runtime_error("Failed to initialize libmagic");
    }

    if (magic_load(magicCookie, "D:/vcpkg/installed/x64-windows/share/libmagic/misc/magic.mgc") != 0) {
        magic_close(magicCookie);
        throw std::runtime_error("Failed to load magic database");
    }
}

FileMetadataExtractor::~FileMetadataExtractor()
{
    magic_close(magicCookie);
}

FileData FileMetadataExtractor::extractMetadata(const fs::directory_entry& entry) {
    std::string name = Utf8Converter::WideToUtf8(entry.path().filename().wstring());
    std::string path = Utf8Converter::WideToUtf8(entry.path().wstring());
    std::string extension = entry.path().extension().string();
    std::string mime_type = getMimeType(entry.path().string());
    long long size = fs::file_size(entry);

    std::string modified_time = getFormattedTime(fs::last_write_time(entry));

    bool is_text = mime_type.rfind("text", 0) == 0 ||
            mime_type.find("pdf") != std::string::npos ||
            mime_type.find("word") != std::string::npos;

    return FileData(name, path, extension, mime_type, modified_time, size, is_text);
}

std::string FileMetadataExtractor::getMimeType(const std::string& filePath) {


    const char* mimeType = magic_file(magicCookie, filePath.c_str());
    std::string result = mimeType ? mimeType : "application/octet-stream";

    return result;
}


std::string FileMetadataExtractor::getFormattedTime(const fs::file_time_type& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t time = std::chrono::system_clock::to_time_t(sctp);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

