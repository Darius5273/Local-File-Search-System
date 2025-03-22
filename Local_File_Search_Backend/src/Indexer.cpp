

#include "../include/Indexer.h"
#include <fstream>
#include <iostream>

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
    generateReport();
}


void Indexer::startCrawlingTimer() {
    startTime = std::chrono::steady_clock::now();
}

void Indexer::endCrawlingTimer() {
    endTime = std::chrono::steady_clock::now();
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
    if (reportFile.is_open()) {
        reportFile << "Indexing Report\n\n";
        reportFile << "Total Files Indexed: " << filesIndexed << "\n";
        reportFile << "Total Files Ignored: " << filesIgnored << "\n";
        reportFile << "Average File Size: " << getAverageFileSize() << " bytes\n";
        reportFile << "Total Time Taken: " << getTotalTimeInSeconds() << " seconds\n";
        reportFile.close();
    } else {
        std::cerr << "Error: Unable to write report to " << reportFilePath << std::endl;
    }
}
