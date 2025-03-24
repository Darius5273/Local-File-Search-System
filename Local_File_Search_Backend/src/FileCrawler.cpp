
#include "../include/FileCrawler.h"
#include "../include/Utf8Converter.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void FileCrawler::scanDirectory() {
    if (!fs::is_directory(rootDirectory)) {
        std::cerr << "Error: " << rootDirectory << " is not a valid directory\n";
        indexer->setDirectoriesScannedToZero();
        return;
    }

    indexer->startCrawlingTimer();

    for (auto it = fs::recursive_directory_iterator(rootDirectory); it != fs::recursive_directory_iterator(); ++it) {
        const auto& entry = *it;
        if (fs::is_directory(entry)) {
            indexer->incrementDirectoriesScanned();
            if (ignoreDirectories.count(entry.path().string()) > 0) {
                indexer->incrementDirectoriesIgnored();
                for (const auto& subEntry : fs::recursive_directory_iterator(entry)) {
                    if (fs::is_regular_file(subEntry) && subEntry.path().filename().string().rfind("~$",0) != 0) {
                        indexer->incrementIgnoredFiles();
                    }
                    if (fs::is_directory(subEntry)) {
                        indexer->incrementDirectoriesIgnored();
                        indexer->incrementDirectoriesScanned();
                    }
                }
                it.disable_recursion_pending();
                continue;
            }
        } else if (fs::is_regular_file(entry)) {
            std::string fileName = Utf8Converter::WideToUtf8(entry.path().filename().wstring());
            bool patternIgnored = false;
            for (const auto& pattern : ignorePatterns) {
                if (fileName.find(pattern) != std::string::npos) {
                    patternIgnored = true;
                    break;
                }
            }
            if (ignoreFiles.count(fileName) > 0 || patternIgnored) {
                indexer->incrementIgnoredFiles();
                continue;
            }

            else if (fileName.rfind("~$",0) == 0)
            {
                continue;
            }

            FileData metadata = fileMetadataExtractor.extractMetadata(entry);
            indexer->addFile(metadata);
            indexer->incrementIndexedFiles(metadata.size);
        }
    }
    indexer->finalizeIndexing();
}

void FileCrawler::setRootDirectory(const std::string& root){
    rootDirectory = root;
}
void FileCrawler::setIgnoreDirectories(const std::unordered_set<std::string>& dirs){
    ignoreDirectories = dirs;
}
void FileCrawler::setIgnoreFiles(const std::unordered_set<std::string>& files){
    ignoreFiles = files;
}

void FileCrawler::setIgnorePatterns(const std::unordered_set<std::string>& patterns){
    ignorePatterns = patterns;
}
