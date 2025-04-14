#include "../include/SearchResult.h"

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText)
        : path(path), score(score), previewText(previewText), is_image(false) {}

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText, const bool& is_image)
        : path(path), score(score), previewText(previewText), is_image(is_image) {}

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