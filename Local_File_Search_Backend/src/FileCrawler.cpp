
#include "../include/FileCrawler.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void FileCrawler::scanDirectory(const std::string& directoryPath) {
    indexer->startCrawlingTimer();

    for (auto it = fs::recursive_directory_iterator(directoryPath); it != fs::recursive_directory_iterator(); ++it) {
        const auto& entry = *it;
        if (fs::is_directory(entry)) {
            if (ignoreDirectories.count(entry.path().filename().string()) > 0) {
                for (const auto& subEntry : fs::recursive_directory_iterator(entry)) {
                    if (fs::is_regular_file(subEntry)) {
                        indexer->incrementIgnoredFiles();
                    }
                }
                it.disable_recursion_pending();
                continue;
            }
        } else if (fs::is_regular_file(entry)) {
            if (ignoreFiles.count(entry.path().filename().string()) > 0) {
                indexer->incrementIgnoredFiles();
                continue;
            }

            FileData metadata = fileMetadataExtractor.extractMetadata(entry);
            indexer->addFile(metadata);
            indexer->incrementIndexedFiles(metadata.size);
        }
    }

    indexer->endCrawlingTimer();
    indexer->finalizeIndexing();
}
