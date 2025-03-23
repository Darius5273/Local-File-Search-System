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
#include <tlhelp32.h>



namespace fs = std::filesystem;

std::string TextExtractor::ExtractTextFromPDF(const std::string& pdfPath) {
    std::string tempOutput = "temp_output.txt";

    std::string command = "pdftotext \"" + pdfPath + "\" \"" + tempOutput + "\" 2>nul";
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr<<"Failed to run pdftotext on: " + pdfPath;
    }

    std::ifstream inFile(tempOutput);
    if (!inFile) {
        std::cerr<<"Failed to open extracted text file";
    }
    std::ostringstream textBuffer;
    textBuffer << inFile.rdbuf();
    inFile.close();

    std::remove(tempOutput.c_str());

    return Utf8Converter::CleanInvalidUtf8(textBuffer.str());
}

DWORD GetProcessIdFromHWND(HWND hwnd) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
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
    std::string text = static_cast<char*>(b);

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

    std::string result;
    std::string line;
    while (getline(file, line)) {

        result += line + '\n';
    }
    return Utf8Converter::CleanInvalidUtf8(result);
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
            std::ostringstream contentBuffer;
            contentBuffer << fileStream.rdbuf();
            return Utf8Converter::CleanInvalidUtf8(contentBuffer.str());
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error processing file " << fileData.path << ": " << ex.what() << std::endl;
        return "";
    }
}
