
#include "../include/CodeWidget.h"

nlohmann::json CodeWidget::toJson() const {
    return {{"icon", "FaCode"}};
}
