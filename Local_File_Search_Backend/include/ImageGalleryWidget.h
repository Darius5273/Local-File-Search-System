#ifndef IMAGEGALLERYWIDGET_H
#define IMAGEGALLERYWIDGET_H

#include <algorithm>
#include "IWidgetObserver.h"

class ImageGalleryWidget : public IWidgetObserver {
private:
    bool active = false;

public:
    void update(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                const std::vector<SearchResult>& results) override;

    bool isActive() const override;
    std::string getName() const override;
};



#endif //IMAGEGALLERYWIDGET_H
