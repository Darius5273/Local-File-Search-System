#include <algorithm>
#include "../include/ImageGalleryWidget.h"

void ImageGalleryWidget::update(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                                const std::vector<SearchResult>& results) {
    int imgCount = 0;
    for (const auto& r : results) {
        if (r.mime_type.find("image/") == 0)
            imgCount++;
    }

    bool queryMentionsImage = parsedQuery.count("path") &&
                                std::any_of(parsedQuery.at("path").begin(), parsedQuery.at("path").end(),
                                          [](const std::string& s) {
                                              return s.find("image") != std::string::npos;
                                          });

    active = imgCount >= 2 && queryMentionsImage;
}

bool ImageGalleryWidget::isActive() const {
    return active;
}

std::string ImageGalleryWidget::getName() const {
    return "View as Gallery";
}