
#ifndef CODEWIDGET_H
#define CODEWIDGET_H
#pragma once
#include "IWidget.h"

class CodeWidget : public IWidget {
public:
    nlohmann::json toJson() const override;
};

#endif //CODEWIDGET_H
