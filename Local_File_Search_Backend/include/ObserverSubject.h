#ifndef CONTEXT_H
#define CONTEXT_H

#include "IObserver.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

class ObserverSubject {
public:
    void attach(IObserver* observer);
    void detach(IObserver* observer);
    void notify();
    void setParsedQuery(std::unordered_map<std::string, std::vector<std::string>> parsedQuery);

private:
    std::vector<IObserver*> observers;
    std::unordered_map<std::string, std::vector<std::string>> parsedQuery;
};

#endif // CONTEXT_H
