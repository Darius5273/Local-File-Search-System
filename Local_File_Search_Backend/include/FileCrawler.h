#ifndef FILE_CRAWLER_H
#define FILE_CRAWLER_H

#include <unordered_set>
#include <string>
#include "Indexer.h"
#include "FileMetadataExtractor.h"

class FileCrawler {
private:
    std::string rootDirectory;
    std::unordered_set<std::string> ignoreDirectories;
    std::unordered_set<std::string> ignoreFiles;
    FileMetadataExtractor fileMetadataExtractor;
    Indexer* indexer;

public:
    FileCrawler(const std::string& root, const std::unordered_set<std::string>& ignoreDirs,
                const std::unordered_set<std::string>& ignoreFiles, Indexer* idx)
            : rootDirectory(root), ignoreDirectories(ignoreDirs), ignoreFiles(ignoreFiles), indexer(idx) {}

    void scanDirectory();
    void setRootDirectory(const std::string& root);
    void setIgnoreDirectories(const std::unordered_set<std::string>& dirs);
    void setIgnoreFiles(const std::unordered_set<std::string>& files);
};

#endif // FILE_CRAWLER_H
