
#include "../include/FileMetadataExtractor.h"
#include "../include/Utf8Converter.h"
#include <fileapi.h>


FileData FileMetadataExtractor::extractMetadata(const fs::directory_entry& entry) {
    std::string name = Utf8Converter::WideToUtf8(entry.path().filename().wstring());
    std::string path = Utf8Converter::WideToUtf8(entry.path().wstring());
    std::string extension = entry.path().extension().string();
    std::string mime_type = getMimeType(extension);
    long long size = fs::file_size(entry);

    std::string modified_time = getFormattedTime(fs::last_write_time(entry));
    std::string created_at = getFileCreationTime(entry.path().wstring());

    bool is_text = mime_type.rfind("text", 0) == 0 ||
            mime_type.find("pdf") != std::string::npos ||
            mime_type.find("word") != std::string::npos;

    return FileData(name, path, extension, mime_type, modified_time, created_at, size, is_text);
}

std::string FileMetadataExtractor::getMimeType(const std::string& extension) {
    if (extension == ".txt" || extension == ".log" || extension == ".cpp") return "text/plain";
    if (extension == ".csv") return "text/csv";
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

std::string FileMetadataExtractor::getFileCreationTime(const std::wstring& filePath) {
    HANDLE hFile = CreateFileW(
            filePath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return "Unavailable";
    }

    FILETIME ftCreate;
    if (!GetFileTime(hFile, &ftCreate, nullptr, nullptr)) {
        CloseHandle(hFile);
        return "Unavailable";
    }

    CloseHandle(hFile);

    ULARGE_INTEGER ull;
    ull.LowPart = ftCreate.dwLowDateTime;
    ull.HighPart = ftCreate.dwHighDateTime;

    const uint64_t WINDOWS_TICK = 10000000ULL;
    const uint64_t SEC_TO_UNIX_EPOCH = 11644473600ULL;

    time_t time = (ull.QuadPart / WINDOWS_TICK) - SEC_TO_UNIX_EPOCH;

    // Convert to localtime
    std::tm local_tm;
    localtime_s(&local_tm, &time);

    std::stringstream ss;
    ss << std::put_time((&local_tm),"%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string FileMetadataExtractor::getFormattedTime(const fs::file_time_type& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t time = std::chrono::system_clock::to_time_t(sctp);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

