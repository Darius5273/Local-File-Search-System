

#include "../include/Indexer.h"
#include <fstream>
#include <iostream>

Indexer::Indexer(DatabaseConnector* db) : dbConnector(db), filesIgnored(0), filesIndexed(0),
                                          totalFileSize(0), totalFilesIndexed(0) {}

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


void Indexer::startCrawlingTimer() {
    dbConnector->connect();
    startTime = std::chrono::steady_clock::now();
}

void Indexer::endCrawlingTimer() {
    endTime = std::chrono::steady_clock::now();
    dbConnector->disconnect();
}

void Indexer::incrementIndexedFiles(long long fileSize) {
    filesIndexed++;
    totalFileSize += fileSize;
}

void Indexer::incrementIgnoredFiles() {
    filesIgnored++;
}

double Indexer::getAverageFileSize() {
    return (filesIndexed > 0) ? static_cast<double>(totalFileSize) / filesIndexed : 0;
}

double Indexer::getTotalTimeInSeconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
}

void Indexer::generateReport() {
    std::ofstream reportFile(reportFilePath);
    std::stringstream reportContent;
    reportContent << "Indexing Report\n\n";
    reportContent << "Total Files Indexed: " << filesIndexed << "\n";
    reportContent << "Total Files Ignored: " << filesIgnored << "\n";
    reportContent << "Average File Size: " << getAverageFileSize() << " bytes\n";
    reportContent << "Total Time Taken: " << getTotalTimeInSeconds() << " seconds\n";

    std::string report = reportContent.str();
    if (reportFile.is_open()) {
        reportFile << report;
        reportFile.close();
    } else {
        std::cerr << "Error: Unable to write report to " << reportFilePath << std::endl;
    }
}
