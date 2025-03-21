
#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>

class SearchResult {
public:
    std::string path;
    std::string previewText;

    SearchResult(const std::string& path, const std::string& previewText);
};

#endif // SEARCH_RESULT_H
