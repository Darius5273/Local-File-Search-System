#include "../include/WidgetManager.h"

void WidgetManager::registerObserver(std::shared_ptr<IWidgetObserver> observer) {
    observers.push_back(std::move(observer));
}

void WidgetManager::updateAll(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                              const std::vector<SearchResult>& results) {
    for (const auto& obs : observers) {
        obs->update(parsedQuery, results);
    }
}

std::vector<std::string> WidgetManager::getActiveWidgets() const {
    std::vector<std::string> active;
    for (const auto& obs : observers) {
        if (obs->isActive()) {
            active.push_back(obs->getName());
        }
    }
    return active;
}
