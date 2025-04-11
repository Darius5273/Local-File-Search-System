
#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>

class SearchResult {
public:
    std::string path;
    double score;
    std::string previewText;

    SearchResult(const std::string& path, const double score, const std::string& previewText);

    const std::string &getPath() const;

    double getScore() const;

    const std::string &getPreviewText() const;

    void setScore(double score);
};

#endif // SEARCH_RESULT_H
