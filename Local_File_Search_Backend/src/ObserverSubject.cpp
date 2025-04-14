
#include "../include/ObserverSubject.h"

void ObserverSubject::attach(IObserver* observer) {
    observers.push_back(observer);
}

void ObserverSubject::detach(IObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void ObserverSubject::notify() {
    for (IObserver* observer : observers) {
        observer->update();
    }
}

