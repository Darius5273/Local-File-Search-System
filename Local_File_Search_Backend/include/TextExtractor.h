#pragma once
#include <string>
#include <vector>
#include "FileData.h"

class TextExtractor {
public:
    static std::string ExtractTextFromPDF(const std::string& pdfPath);

    static std::string ExtractTextFromWord(const std::wstring& docPath);

    static std::string ParseCSV(const std::string& csvPath);

    static std::string GetFileContent(const FileData& fileData);

};
