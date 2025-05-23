#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H
#pragma once
#include "IWidgetObserver.h"
#include <vector>
#include <string>
#include <memory>

#include "IWidgetObserver.h"
#include <vector>
#include <memory>

class WidgetManager {
private:
    std::vector<std::shared_ptr<IWidgetObserver>> observers;

public:
    void registerObserver(std::shared_ptr<IWidgetObserver> observer);
    void updateAll(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                   const std::vector<SearchResult>& results);
    std::vector<std::string> getActiveWidgets() const;
};
#endif //WIDGETMANAGER_H
