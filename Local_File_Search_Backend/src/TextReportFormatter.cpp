#include "../include/TextReportFormatter.h"
#include <sstream>
#include <climits>
#include <fstream>
#include <iostream>

void TextReportFormatter::formatAndSave(const std::map<std::string, std::string>& reportData) const {
    std::ofstream file(reportFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to write report to " << reportFilePath << std::endl;
        return;
    }

    std::stringstream ss;
    ss << "Indexing Report\n\n";
    for (const auto& [key, value] : reportData) {
        ss << key << ": " << value << "\n";
    }

    file << ss.str();
    file.close();
}
