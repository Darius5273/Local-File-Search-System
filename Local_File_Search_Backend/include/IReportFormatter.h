#ifndef IREPORTFORMATTER_H
#define IREPORTFORMATTER_H

#include <string>
#include "Indexer.h"

class IReportFormatter {
public:
    virtual ~IReportFormatter() = default;

    virtual void formatAndSave(const Indexer& indexer) const = 0;
};

#endif // IREPORTFORMATTER_H
