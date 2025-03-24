#include "DatabaseConnector.h"

class Indexer {
private:
    DatabaseConnector* dbConnector;
    std::vector<FileData> fileBatch;
    int batchSize = 100;
    int totalFilesIndexed = 0;
    std::vector<std::string> indexedFiles;
    std::string reportFilePath = "../../index_progress_report.txt";
    int filesIndexed = 0;
    int filesIgnored = 0;
    long long totalFileSize = 0;
    long long maxFileSize = 0;
    long long minFileSize = LLONG_MAX;
    int totalDirectoriesScanned = 0;
    int totalFilesProcessed = 0;
    int totalDirectoriesIgnored = 0;
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
    void incrementDirectoriesScanned();
    void incrementDirectoriesIgnored();
    double getAverageFileSize();
    double getTotalTimeInMilliseconds();
};
