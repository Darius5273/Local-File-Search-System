#ifndef MARKDOWNREPORTFORMATTER_H
#define MARKDOWNREPORTFORMATTER_H

#include "IReportFormatter.h"

class MarkdownReportFormatter : public IReportFormatter {

private:
    const std::string reportFilePath = "../../index_progress_report.md";
public:
    void formatAndSave(const std::map<std::string, std::string>& reportData) const override;



};

#endif // MARKDOWNREPORTFORMATTER_H
