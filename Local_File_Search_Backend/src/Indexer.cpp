

#include "../include/Indexer.h"
#include <fstream>
#include <iostream>

Indexer::Indexer(DatabaseConnector* db) : dbConnector(db) {}

void Indexer::addFile(const FileData& file) {
    fileBatch.push_back(file);
    indexedFiles.push_back(file.path);
    if (fileBatch.size() >= batchSize) {
        processBatch();
    }
}

void Indexer::processBatch() {
    dbConnector->insertBatch(fileBatch);
    totalFilesIndexed += fileBatch.size();
    fileBatch.clear();
}

void Indexer::finalizeIndexing() {
    if (!fileBatch.empty()) processBatch();
    endCrawlingTimer();
    generateReport();
}

void Indexer::setDirectoriesScannedToZero()
{
    totalDirectoriesScanned = 0;
}

void Indexer::startCrawlingTimer() {
    dbConnector->connect();
    filesIndexed = 0;
    filesIndexed = 0;
    filesIgnored = 0;
    totalFileSize = 0;
    maxFileSize = 0;
    minFileSize = LLONG_MAX;
    totalDirectoriesScanned = 1;
    totalFilesProcessed = 0;
    totalDirectoriesIgnored = 0;

    startTime = std::chrono::steady_clock::now();
}

void Indexer::endCrawlingTimer() {
    endTime = std::chrono::steady_clock::now();
    dbConnector->disconnect();
}

void Indexer::incrementIndexedFiles(long long fileSize) {
    filesIndexed++;
    totalFilesProcessed++;
    totalFileSize += fileSize;
    if (fileSize > maxFileSize) maxFileSize = fileSize;
    if (fileSize > 0 && fileSize < minFileSize) minFileSize = fileSize;
}

void Indexer::incrementIgnoredFiles() {
    filesIgnored++;
    totalFilesProcessed++;
}

void Indexer::incrementDirectoriesScanned() {
    totalDirectoriesScanned++; }

void Indexer::incrementDirectoriesIgnored() {
    totalDirectoriesIgnored++; }


double Indexer::getAverageFileSize() {
    return (filesIndexed > 0) ? static_cast<double>(totalFileSize) / filesIndexed : 0;
}

double Indexer::getTotalTimeInMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}

void Indexer::generateReport() {
    std::ofstream reportFile(reportFilePath);
    std::stringstream reportContent;
    reportContent << "Indexing Report\n\n";
    reportContent << "Total Files Indexed: " << filesIndexed << "\n";
    reportContent << "Total Files Ignored: " << filesIgnored << "\n";
    reportContent << "Total Files Processed: " << totalFilesProcessed << "\n";
    reportContent << "Total Directories Scanned: " << totalDirectoriesScanned<< "\n";
    reportContent << "Total Directories Ignored: " << totalDirectoriesIgnored << "\n";
    reportContent << "Average File Size: " << getAverageFileSize() << " bytes\n";
    reportContent<< "Maximum File Size: " << maxFileSize << " bytes\n";
    reportContent << "Minimum File Size: " << (minFileSize == LLONG_MAX ? 0 : minFileSize) << " bytes\n";
    reportContent << "Total Time Taken: " << getTotalTimeInMilliseconds() << " milliseconds\n";

    std::string report = reportContent.str();
    if (reportFile.is_open()) {
        reportFile << report;
        reportFile.close();
    } else {
        std::cerr << "Error: Unable to write report to " << reportFilePath << std::endl;
    }
}
