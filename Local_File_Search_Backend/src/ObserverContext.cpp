
#include "../include/ObserverContext.h"

void ObserverContext::attach(IObserver* observer) {
    observers.push_back(observer);
}

void ObserverContext::detach(IObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void ObserverContext::notify() {
    for (IObserver* observer : observers) {
        observer->update(parsedQuery);
    }
}

void ObserverContext::setParsedQuery(std::unordered_map<std::string, std::vector<std::string>> parsedQuery){
    ObserverContext::parsedQuery = parsedQuery;
    notify();
}
