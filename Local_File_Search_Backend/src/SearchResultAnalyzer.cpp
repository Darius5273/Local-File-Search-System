#include <sstream>
#include <map>
#include <regex>
#include <nlohmann/json.hpp>
#include "../include/SearchResultAnalyzer.h"

using json = nlohmann::json;

namespace {
    std::string mimeToCategory(const std::string& mime) {
        if (mime.find("image/") == 0) return "Image";
        if (mime == "application/pdf") return "PDF";
        if (mime == "application/vnd.openxmlformats-officedocument.wordprocessingml.document") return "DOCX";
        if (mime == "application/vnd.openxmlformats-officedocument.presentationml.presentation") return "PPTX";
        if (mime == "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet") return "Excel";
        if (mime.find("text/") == 0) return "Text";
        return "Other";
    }

    std::string sizeToBucket(int size) {
        if (size < 1024) return "Small (<1KB)";
        else if (size < 1024 * 1024) return "Medium (1KB–1MB)";
        else return "Large (>1MB)";
    }

    std::string extractLanguageHint(const std::string& path) {
        std::regex langRegex(R"(\.(c|cpp|h|java|py|js|jsx|ts|tsx|html|css|pl)$)", std::regex::icase);
        std::smatch match;
        if (std::regex_search(path, match, langRegex)) {
            std::string ext = match[1];
            if (ext == "c") return "C";
            if (ext == "cpp" || ext == "h") return "C++";
            if (ext == "java") return "Java";
            if (ext == "py") return "Python";
            if (ext == "js" || ext == "jsx") return "JavaScript";
            if (ext == "ts" || ext == "tsx") return "TypeScript";
            if (ext == "html") return "HTML";
            if (ext == "css") return "CSS";
            if (ext == "pl") return "Prolog";
        }
        return "";
    }
}

nlohmann::json SearchResultAnalyzer::analyzeMetadata(const std::vector<SearchResult>& results) const {
    std::unordered_map<std::string, int> mimeCategories;
    std::unordered_map<std::string, int> extensionCount;
    std::unordered_map<std::string, int> yearCount;
    std::unordered_map<std::string, int> sizeBuckets;
    std::unordered_map<std::string, int> languages;

    uint64_t totalSize = 0;

    for (const auto& r : results) {
        ++mimeCategories[mimeToCategory(r.mime_type)];
        auto ext = r.extension.substr(1);
        ++extensionCount[ext];

        if (r.modified_time.size() >= 4) {
            std::string year = r.modified_time.substr(0, 4);
            ++yearCount[year];
        }

        ++sizeBuckets[sizeToBucket(r.size)];
        totalSize += r.size;

        std::string lang = extractLanguageHint(r.path);
        if (!lang.empty()) {
            ++languages[lang];
        }
    }

    json summary;

    for (const auto& [cat, count] : mimeCategories)
        summary["fileTypeCategories"].push_back({ {"type", cat}, {"count", count} });

    for (const auto& [ext, count] : extensionCount)
        summary["extensionCounts"].push_back({ {"extension", ext}, {"count", count} });

    for (const auto& [year, count] : yearCount)
        summary["yearCounts"].push_back({ {"year", year}, {"count", count} });

    for (const auto& [bucket, count] : sizeBuckets)
        summary["sizeBuckets"].push_back({ {"bucket", bucket}, {"count", count} });

    if (!languages.empty()) {
        for (const auto& [lang, count] : languages)
            summary["languages"].push_back({ {"language", lang}, {"count", count} });
    }

    return summary;
}
