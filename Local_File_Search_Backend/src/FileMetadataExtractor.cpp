
#include "../include/FileMetadataExtractor.h"

FileData FileMetadataExtractor::extractMetadata(const fs::directory_entry& entry) {
    std::string name = entry.path().filename().string();
    std::string path = entry.path().string();
    std::string extension = entry.path().extension().string();
    std::string mime_type = getMimeType(extension);
    long long size = fs::file_size(entry);

    std::string modified_time = getFormattedTime(fs::last_write_time(entry));
    std::string created_at = getFormattedTime(fs::last_write_time(entry));

    bool is_text = mime_type.rfind("text", 0) == 0;

    return FileData(name, path, extension, mime_type, modified_time, created_at, size, is_text);
}

std::string FileMetadataExtractor::getMimeType(const std::string& extension) {
    if (extension == ".txt" || extension == ".log" || extension == ".csv") return "text/plain";
    if (extension == ".pdf") return "application/pdf";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    if (extension == ".png") return "image/png";
    if (extension == ".mp4") return "video/mp4";
    if (extension == ".mp3") return "audio/mp3";
    if (extension == ".xls") return "application/vnd.ms-excel";
    if (extension == ".xlsx") return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    if (extension == ".ppt") return "application/vnd.ms-powerpoint";
    if (extension == ".pptx") return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    if (extension == ".doc") return "application/msword";
    if (extension == ".docx") return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";

    return "application/octet-stream";
}

std::string FileMetadataExtractor::getFormattedTime(const fs::file_time_type& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t time = std::chrono::system_clock::to_time_t(sctp);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
