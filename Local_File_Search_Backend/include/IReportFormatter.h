#ifndef IREPORTFORMATTER_H
#define IREPORTFORMATTER_H

#include <string>
#include <map>

class IReportFormatter {
public:
    virtual ~IReportFormatter() = default;

    virtual void formatAndSave(const std::map<std::string, std::string>& reportData) const = 0;
};

#endif // IREPORTFORMATTER_H
