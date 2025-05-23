
#ifndef IWIDGET_H
#define IWIDGET_H
#pragma once
#include <nlohmann/json.hpp>

class IWidget {
public:
    virtual ~IWidget() = default;
    virtual nlohmann::json toJson() const = 0;
};

#endif //IWIDGET_H
