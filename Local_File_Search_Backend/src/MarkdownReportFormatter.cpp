#include <sstream>
#include <climits>
#include <fstream>
#include <iostream>
#include "../include/MarkdownReportFormatter.h"

void MarkdownReportFormatter::formatAndSave(const Indexer& indexer) const {
    std::ofstream file(reportFilePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to write report to " << reportFilePath << std::endl;
        return;
    }
    std::stringstream ss;
    ss << "# Indexing Report\n\n";
    ss << "**Total Files Indexed:** " << indexer.getFilesIndexed() << "  \n";
    ss << "**Total Files Ignored:** " << indexer.getFilesIgnored() << "  \n";
    ss << "**Total Files Processed:** " << indexer.getTotalFilesProcessed() << "  \n";
    ss << "**Total Directories Scanned:** " << indexer.getTotalDirectoriesScanned() << "  \n";
    ss << "**Total Directories Ignored:** " << indexer.getTotalDirectoriesIgnored() << "  \n";
    ss << "**Average File Size:** " << indexer.getAverageFileSize() << " bytes  \n";
    ss << "**Maximum File Size:** " << indexer.getMaxFileSize() << " bytes  \n";
    ss << "**Minimum File Size:** " << (indexer.getMinFileSize() == LLONG_MAX ? 0 : indexer.getMinFileSize()) << " bytes  \n";
    ss << "**Total Time Taken:** " << indexer.getTotalTimeInMilliseconds() << " milliseconds  \n";

    file << ss.str();
    file.close();
}
