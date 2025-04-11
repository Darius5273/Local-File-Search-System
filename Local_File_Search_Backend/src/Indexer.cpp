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

void Indexer::setReportFormatter(std::unique_ptr<IReportFormatter> formatter) {
    reportFormatter = std::move(formatter);
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


double Indexer::getAverageFileSize() const  {
    return (filesIndexed > 0) ? static_cast<double>(totalFileSize) / filesIndexed : 0;
}

double Indexer::getTotalTimeInMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}


void Indexer::generateReport() {
    std::map<std::string, std::string> reportData;

    reportData["Total Files Indexed"] = std::to_string(getFilesIndexed());
    reportData["Total Files Ignored"] = std::to_string(getFilesIgnored());
    reportData["Total Files Processed"] = std::to_string(getTotalFilesProcessed());
    reportData["Total Directories Scanned"] = std::to_string(getTotalDirectoriesScanned());
    reportData["Total Directories Ignored"] = std::to_string(getTotalDirectoriesIgnored());
    reportData["Average File Size"] = std::to_string(getAverageFileSize()) + " bytes";
    reportData["Maximum File Size"] = std::to_string(getMaxFileSize()) + " bytes";
    reportData["Minimum File Size"] = std::to_string(minFileSize == LLONG_MAX ? 0 : minFileSize) + " bytes";
    reportData["Total Time Taken"] = std::to_string(getTotalTimeInMilliseconds()) + " milliseconds";

    reportFormatter->formatAndSave(reportData);
}

int Indexer::getFilesIndexed() const {
    return filesIndexed;
}

int Indexer::getFilesIgnored() const {
    return filesIgnored;
}

long long int Indexer::getMaxFileSize() const {
    return maxFileSize;
}

long long int Indexer::getMinFileSize() const {
    return minFileSize;
}

int Indexer::getTotalDirectoriesScanned() const {
    return totalDirectoriesScanned;
}

int Indexer::getTotalFilesProcessed() const {
    return totalFilesProcessed;
}

int Indexer::getTotalDirectoriesIgnored() const {
    return totalDirectoriesIgnored;
}

