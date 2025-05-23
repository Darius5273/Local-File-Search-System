#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H
#pragma once
#include "IWidget.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class WidgetFactory {
public:
    std::vector<std::shared_ptr<IWidget>> createWidgets(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);
};

#endif //WIDGETFACTORY_H
