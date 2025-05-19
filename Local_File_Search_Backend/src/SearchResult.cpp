#include "../include/SearchResult.h"

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText)
        : path(path), score(score), previewText(previewText), is_image(false) {}

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText, const bool& is_image)
        : path(path), score(score), previewText(previewText), is_image(is_image) {}

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText,
             const int size, const std::string& mime_type, const std::string& extension, const std::string& modified_time)
             : path(path), score(score), previewText(previewText), is_image(false), size(size), mime_type(mime_type),
             modified_time(modified_time), extension(extension) {};

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText, const bool& is_image,
                           const int size, const std::string& mime_type, const std::string& extension, const std::string& modified_time)
        : path(path), score(score), previewText(previewText), is_image(is_image), size(size), mime_type(mime_type),
          modified_time(modified_time), extension(extension) {};

const std::string &SearchResult::getPath() const {
    return path;
}

double SearchResult::getScore() const {
    return score;
}

void SearchResult::setScore(double score) {
    SearchResult::score = score;
}

const std::string &SearchResult::getPreviewText() const {
    return previewText;
}