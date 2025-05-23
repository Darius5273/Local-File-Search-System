
#ifndef PROGRAMMINGLANGUAGEWIDGET_H
#define PROGRAMMINGLANGUAGEWIDGET_H
#include "IWidgetObserver.h"

class ProgrammingLanguageWidget : public IWidgetObserver {
private:
    bool active = false;

public:
    void update(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                const std::vector<SearchResult>& results) override;

    bool isActive() const override;
    std::string getName() const override;
};

#endif //PROGRAMMINGLANGUAGEWIDGET_H
