#include "DatabaseConnector.h"

class Indexer {
private:
    DatabaseConnector* dbConnector;
    std::vector<FileData> fileBatch;
    int batchSize = 100;
    int totalFilesIndexed;
    std::vector<std::string> indexedFiles;
    std::string reportFilePath = "../../index_progress_report.txt";
    int filesIndexed;
    int filesIgnored;
    long long totalFileSize;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

public:
    Indexer(DatabaseConnector* db);

    void addFile(const FileData& file);
    void processBatch();
    void finalizeIndexing();
    void generateReport();
    void startCrawlingTimer();
    void endCrawlingTimer();
    void incrementIndexedFiles(long long fileSize);
    void incrementIgnoredFiles();
    double getAverageFileSize();
    double getTotalTimeInSeconds();
};
