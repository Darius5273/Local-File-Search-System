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

    static std::string CleanInvalidUtf8(const std::string str) {
        std::string cleanStr;
        cleanStr.reserve(str.size());

        for (int i = 0; i < str.size();) {
            unsigned char c = str[i];

            if ((c & 0x80) == 0) {
                cleanStr += c;
                i++;
            }
            else if ((c & 0xE0) == 0xC0 && i + 1 < str.size() && (str[i + 1] & 0xC0) == 0x80) {
                cleanStr += c;
                cleanStr += str[i + 1];
                i += 2;
            }
            else if ((c & 0xF0) == 0xE0 && i + 2 < str.size() &&
                     (str[i + 1] & 0xC0) == 0x80 && (str[i + 2] & 0xC0) == 0x80) {
                cleanStr += c;
                cleanStr += str[i + 1];
                cleanStr += str[i + 2];
                i += 3;
            }
            else if ((c & 0xF8) == 0xF0 && i + 3 < str.size() &&
                     (str[i + 1] & 0xC0) == 0x80 && (str[i + 2] & 0xC0) == 0x80 && (str[i + 3] & 0xC0) == 0x80) {
                cleanStr += c;
                cleanStr += str[i + 1];
                cleanStr += str[i + 2];
                cleanStr += str[i + 3];
                i += 4;
            }
            else {
                cleanStr += '?';
                i++;
            }
        }

        return cleanStr;
    }
};
