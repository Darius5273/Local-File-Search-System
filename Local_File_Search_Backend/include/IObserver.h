#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "SearchResult.h"

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

#endif // IOBSERVER_H
