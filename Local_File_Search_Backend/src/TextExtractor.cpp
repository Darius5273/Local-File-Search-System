#include "../include/TextExtractor.h"
#include "../include/FileData.h"
#include "../include/Utf8Converter.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <windows.h>
#include <comdef.h>
#include <comutil.h>
#include <iostream>
#include <filesystem>
#include <minwindef.h>

const size_t CHUNK_SIZE = 64 * 1024;
const size_t MAX_SIZE = 0.88 * 1024 * 1024;
const std::string tempOutput = "temp_output.txt";


namespace fs = std::filesystem;

std::string TextExtractor::ExtractTextFromPDF(const std::string& pdfPath) {

    std::string command = "pdftotext \"" + pdfPath + "\" \"" + tempOutput + "\" 2>nul";
    int result = system(command.c_str());

    if (result != 0) {
        throw std::runtime_error("Failed to run pdftotext on: " + pdfPath + "\n");
    }

    std::ifstream inFile(tempOutput);
    if (!inFile) {
        throw std::runtime_error("Failed to open extracted text file: " + tempOutput);
    }
    std::ostringstream textBuffer;
    char buffer[CHUNK_SIZE];
    size_t totalBytesRead = 0;

    while (totalBytesRead < MAX_SIZE && inFile) {
        inFile.read(buffer, min(CHUNK_SIZE, MAX_SIZE - totalBytesRead));
        size_t bytesRead = inFile.gcount();

        if (bytesRead > 0) {
            textBuffer.write(buffer, bytesRead);
            totalBytesRead += bytesRead;
        }
    }
    inFile.close();

    std::remove(tempOutput.c_str());

    return Utf8Converter::CleanInvalidUtf8(textBuffer.str());
}

std::string TextExtractor::ExtractTextFromWord(const std::wstring& docPath) {
    CoInitialize(NULL);
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(L"Word.Application", &clsid);
    if (FAILED(hr)) {
        CoUninitialize();
        throw std::runtime_error("Failed to get Word.Application CLSID.");
    }

    IDispatch* pWordApp = nullptr;
    hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pWordApp);
    if (FAILED(hr) || pWordApp == nullptr) {
        CoUninitialize();
        throw std::runtime_error("Failed to start Word COM.");
    }


    DISPID dispID;
    OLECHAR* name = L"Documents";
    pWordApp->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispID);

    VARIANT result;
    VariantInit(&result);
    DISPPARAMS noArgs = { nullptr, nullptr, 0, 0 };
    hr = pWordApp->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &result, nullptr, nullptr);
    if (FAILED(hr)) {
        pWordApp->Release();
        CoUninitialize();
        throw std::runtime_error("Failed to get Documents collection.");
    }


    IDispatch* pDocs = result.pdispVal;

    VARIANT fileName;
    fileName.vt = VT_BSTR;
    fileName.bstrVal = SysAllocString(docPath.c_str());

    VARIANT docResult;
    VariantInit(&docResult);
    DISPID openID;
    OLECHAR* openName = L"Open";
    pDocs->GetIDsOfNames(IID_NULL, &openName, 1, LOCALE_USER_DEFAULT, &openID);

    DISPPARAMS params = { &fileName, nullptr, 1, 0 };
    pDocs->Invoke(openID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &docResult, nullptr, nullptr);


    IDispatch* pDoc = docResult.pdispVal;

    VARIANT contentResult;
    VariantInit(&contentResult);
    DISPID contentID;
    OLECHAR* contentName = L"Content";
    pDoc->GetIDsOfNames(IID_NULL, &contentName, 1, LOCALE_USER_DEFAULT, &contentID);

    hr = pDoc->Invoke(contentID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &contentResult, nullptr, nullptr);
    if (FAILED(hr)) {
        pDoc->Release();
        pDocs->Release();
        pWordApp->Release();
        CoUninitialize();
        throw std::runtime_error("Failed to get document content.");
    }

    IDispatch* pContent = contentResult.pdispVal;
    DISPID textID;
    OLECHAR* textName = L"Text";
    pContent->GetIDsOfNames(IID_NULL, &textName, 1, LOCALE_USER_DEFAULT, &textID);

    VARIANT textResult;
    VariantInit(&textResult);
    hr = pContent->Invoke(textID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &textResult, nullptr, nullptr);
    if (FAILED(hr)) {
        pContent->Release();
        pDoc->Release();
        pDocs->Release();
        pWordApp->Release();
        CoUninitialize();
        throw std::runtime_error("Failed to get document text.");
    }

    _bstr_t b(textResult.bstrVal);
    const char *rawText = static_cast<char*>(b);

    const size_t MAX_SIZE = 1024 * 1024;
    size_t textLength = strlen(rawText);
    size_t sizeToCopy = (textLength > MAX_SIZE) ? MAX_SIZE : textLength;
    std::string text(rawText, sizeToCopy);

    DISPID quitID;
    OLECHAR* quitName = L"Quit";
    pWordApp->GetIDsOfNames(IID_NULL, &quitName, 1, LOCALE_USER_DEFAULT, &quitID);
    pWordApp->Invoke(quitID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &noArgs, nullptr, nullptr, nullptr);

    pContent->Release();
    pDoc->Release();
    pDocs->Release();
    if (pWordApp) pWordApp->Release();
    VariantClear(&textResult);
    VariantClear(&fileName);
    CoUninitialize();




    return Utf8Converter::CleanInvalidUtf8(text);
}

std::string TextExtractor::ParseCSV(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open())
        std::cerr<<"Cannot open CSV file: " << csvPath;


    std::ostringstream resultBuffer;
    std::string line;
    size_t totalBytesRead = 0;

    while (std::getline(file, line) && totalBytesRead < MAX_SIZE) {
        if (totalBytesRead + line.size() + 1 > MAX_SIZE) {
            line = line.substr(0, MAX_SIZE - totalBytesRead - 1);
        }
        resultBuffer << line << '\n';
        totalBytesRead += line.size() + 1;
    }

    std::string text = resultBuffer.str();

    return Utf8Converter::CleanInvalidUtf8(text);
}


std::string TextExtractor::GetFileContent(const FileData& fileData) {
    std::string mimeType = fileData.mime_type;

    try {
        if (mimeType.find("pdf") != std::string::npos) {
            return ExtractTextFromPDF(fileData.path);
        }
        else if (mimeType.find("word") != std::string::npos || mimeType.find(".docs") != std::string::npos) {
            return ExtractTextFromWord((Utf8Converter::Utf8ToWide(fileData.path)));
        }
        else if (mimeType.find("csv") != std::string::npos) {
            return ParseCSV(fileData.path);
        }
        else {
            std::ifstream fileStream(fileData.path);
            if (!fileStream)
                std::cerr <<"Failed to open file";
            std::ostringstream textBuffer;
            char buffer[CHUNK_SIZE];
            size_t totalBytesRead = 0;

            while (totalBytesRead < MAX_SIZE && fileStream) {
                fileStream.read(buffer, min(CHUNK_SIZE, MAX_SIZE - totalBytesRead));
                size_t bytesRead = fileStream.gcount();

                if (bytesRead > 0) {
                    textBuffer.write(buffer, bytesRead);
                    totalBytesRead += bytesRead;
                }
            }
            return Utf8Converter::CleanInvalidUtf8(textBuffer.str());
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error processing file " << fileData.path << ": " << ex.what() << std::endl;
        return "";
    }
}
