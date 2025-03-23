
#include "../include/FileCrawler.h"
#include "../include/Utf8Converter.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void FileCrawler::scanDirectory() {
    indexer->startCrawlingTimer();

    for (auto it = fs::recursive_directory_iterator(rootDirectory); it != fs::recursive_directory_iterator(); ++it) {
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
            if (ignoreFiles.count(Utf8Converter::WideToUtf8(entry.path().filename().wstring())) > 0) {
                indexer->incrementIgnoredFiles();
                continue;
            }

            FileData metadata = fileMetadataExtractor.extractMetadata(entry);
            indexer->addFile(metadata);
            indexer->incrementIndexedFiles(metadata.size);
        }
    }
    indexer->finalizeIndexing();
}

void print_unordered_set(const std::unordered_set<std::string>& set) {
    for (const auto& element : set) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

void FileCrawler::setRootDirectory(const std::string& root){
    rootDirectory = root;
    std::cout<<root;
}
void FileCrawler::setIgnoreDirectories(const std::unordered_set<std::string>& dirs){
    ignoreDirectories = dirs;
    print_unordered_set(ignoreDirectories);
}
void FileCrawler::setIgnoreFiles(const std::unordered_set<std::string>& files){
    ignoreFiles = files;
    print_unordered_set(ignoreFiles);
}

