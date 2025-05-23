
#ifndef COMPUTERWIDGET_H
#define COMPUTERWIDGET_H
#pragma once
#include "IWidget.h"

class ComputerWidget : public IWidget {
public:
    nlohmann::json toJson() const override;
};

#endif //COMPUTERWIDGET_H
