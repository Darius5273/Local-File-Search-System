#pragma once
#include <Windows.h>
#include <string>
#include <stdexcept>

class Utf8Converter {
public:
    static std::string WideToUtf8(const std::wstring& wstr) {
        int len = WideCharToMultiByte(
                CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()),
                nullptr, 0, nullptr, nullptr
        );

        if (len == 0) {
            throw std::runtime_error("Wide to UTF-8 conversion failed. Error: " + std::to_string(GetLastError()));
        }

        std::string utf8Str(len, 0);
        WideCharToMultiByte(
                CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()),
                utf8Str.data(), len, nullptr, nullptr
        );

        return utf8Str;
    }

    static std::wstring Utf8ToWide(const std::string& str) {
        int len = MultiByteToWideChar(
                CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()),
                nullptr, 0
        );

        if (len == 0) {
            throw std::runtime_error("UTF-8 to wide conversion failed. Error: " + std::to_string(GetLastError()));
        }

        std::wstring wideStr(len, 0);
        MultiByteToWideChar(
                CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()),
                wideStr.data(), len
        );

        return wideStr;
    }
};
