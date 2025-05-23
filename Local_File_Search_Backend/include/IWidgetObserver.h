#ifndef IWIDGETOBSERVER_H
#define IWIDGETOBSERVER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "SearchResult.h"

#include <string>
#include <vector>
#include <unordered_map>
#include "SearchResult.h"

class IWidgetObserver {
public:
    virtual ~IWidgetObserver() = default;

    virtual void update(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery,
                        const std::vector<SearchResult>& results) = 0;

    virtual bool isActive() const = 0;
    virtual std::string getName() const = 0;
};
#endif //IWIDGETOBSERVER_H
