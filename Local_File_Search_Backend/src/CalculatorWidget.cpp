
#include "../include/CalculatorWidget.h"

nlohmann::json CalculatorWidget::toJson() const {
    return {{"icon", "FaCalculator"}};
}
