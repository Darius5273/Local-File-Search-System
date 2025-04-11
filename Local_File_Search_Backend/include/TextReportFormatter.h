#ifndef TEXTREPORTFORMATTER_H
#define TEXTREPORTFORMATTER_H

#include "IReportFormatter.h"

class TextReportFormatter : public IReportFormatter {
private:
    const std::string reportFilePath = "../../index_progress_report.txt";
public:
    void formatAndSave(const std::map<std::string, std::string>& reportData) const override;

};

#endif // TEXTREPORTFORMATTER_H
