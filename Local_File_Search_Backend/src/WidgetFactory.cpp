#include "../include/WidgetFactory.h"
#include "../include/ComputerWidget.h"
#include "../include/CalculatorWidget.h"
#include "../include/CodeWidget.h"

bool containsKeyword(const std::unordered_map<std::string, std::vector<std::string>>& query, const std::string& keyword) {
        for (const auto& [field, values] : query) {
            for (const auto& value : values) {
                if (value.find(keyword) != std::string::npos) {
                    return true;
                }
            }
        }
        return false;
}


std::vector<std::shared_ptr<IWidget>> WidgetFactory::createWidgets(
        const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {

    std::vector <std::shared_ptr<IWidget>> widgets;

    if (containsKeyword(parsedQuery, "computer")) {
        widgets.push_back(std::make_shared<ComputerWidget>());
    }
    if (containsKeyword(parsedQuery, "calculator")) {
        widgets.push_back(std::make_shared<CalculatorWidget>());
    }
    if (containsKeyword(parsedQuery, "code")) {
        widgets.push_back(std::make_shared<CodeWidget>());
    }

    return widgets;
}
