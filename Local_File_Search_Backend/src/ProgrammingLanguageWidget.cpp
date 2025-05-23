#include <unordered_set>
#include <algorithm>
#include "../include/ProgrammingLanguageWidget.h"

void ProgrammingLanguageWidget::update(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                                       const std::vector<SearchResult>& results) {
    std::unordered_set<std::string> codeMimes = {
            "text/x-c", "text/x-c++", "text/x-java", "application/javascript",
            "text/x-python", "application/x-typescript", "text/css", "text/html",
            "text/x-shellscript"
    };
    const std::vector<std::string> codeExtensions = {
            ".cpp", ".c", ".h", ".py", ".java", ".js", ".ts", ".pl", ".sh"
    };

    int codeCount = 0;
    for (const auto& r : results) {
        std::string ext = r.extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (codeMimes.count(r.mime_type) || std::find(codeExtensions.begin(), codeExtensions.end(), ext) != codeExtensions.end()) {
            codeCount++;
        }
    }

    bool queryImpliesCode = false;
    if (parsedQuery.count("content")) {
        for (const auto& word : parsedQuery.at("content")) {
            std::string w = word;
            std::transform(w.begin(), w.end(), w.begin(), ::tolower);
            if (w.find("code") != std::string::npos ||
                w.find("class") != std::string::npos ||
                w.find("func") != std::string::npos ||
                w.find("def") != std::string::npos ||
                w.find("var") != std::string::npos ||
                w.find("++") != std::string::npos) {
                queryImpliesCode = true;
                break;
            }
        }
    }

    active = codeCount >= 2 && queryImpliesCode;
}

bool ProgrammingLanguageWidget::isActive() const {
    return active;
}

std::string ProgrammingLanguageWidget::getName() const {
    return "Analyze Code Snippets";
}
