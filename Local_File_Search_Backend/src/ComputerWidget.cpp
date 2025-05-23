
#include "../include/ComputerWidget.h"

nlohmann::json ComputerWidget::toJson() const {
    return {{"icon", "FaLaptop"}};
}
