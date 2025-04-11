#include "../include/SearchResult.h"

SearchResult::SearchResult(const std::string& path, const double score, const std::string& previewText)
        : path(path), score(score), previewText(previewText) {}

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