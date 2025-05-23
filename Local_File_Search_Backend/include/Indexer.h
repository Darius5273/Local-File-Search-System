#include "DatabaseConnector.h"
#include "IReportFormatter.h"

class Indexer {
private:
    DatabaseConnector* dbConnector;
    std::vector<FileData> fileBatch;
    int batchSize = 100;
    int totalFilesIndexed = 0;
    std::vector<std::string> indexedFiles;
    std::unique_ptr<IReportFormatter> reportFormatter;
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

    void setReportFormatter(std::unique_ptr<IReportFormatter> formatter);
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
    void setDirectoriesScannedToZero();
    double getAverageFileSize() const;
    double getTotalTimeInMilliseconds() const;
    int getFilesIndexed() const;
    int getFilesIgnored() const;
    long long int getMaxFileSize() const;
    long long int getMinFileSize() const;
    int getTotalDirectoriesScanned() const;
    int getTotalFilesProcessed() const;
    int getTotalDirectoriesIgnored() const;

};
