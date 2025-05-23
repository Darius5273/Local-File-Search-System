
#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H
#pragma once
#include "IWidget.h"

class CalculatorWidget : public IWidget {
public:
    nlohmann::json toJson() const override;
};

#endif //CALCULATORWIDGET_H
