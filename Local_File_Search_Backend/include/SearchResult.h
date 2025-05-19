
#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>

class SearchResult {
public:
    std::string path;
    double score;
    std::string previewText;
    bool is_image;
    int size;
    std::string mime_type;
    std::string extension;
    std::string modified_time;

    SearchResult(const std::string& path, const double score, const std::string& previewText);
    SearchResult(const std::string& path, const double score, const std::string& previewText,
                 const int size, const std::string& mime_type, const std::string& extension, const std::string& modified_time);
    SearchResult(const std::string& path, const double score, const std::string& previewText, const bool& is_image);
    SearchResult(const std::string& path, const double score, const std::string& previewText, const bool& is_image,
                 const int size, const std::string& mime_type, const std::string& extension, const std::string& modified_time);

    const std::string &getPath() const;

    double getScore() const;

    const std::string &getPreviewText() const;

    void setScore(double score);
};

#endif // SEARCH_RESULT_H
