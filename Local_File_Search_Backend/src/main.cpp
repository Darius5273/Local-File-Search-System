#include <iostream>
#include <unordered_set>
#include "../include/DatabaseConnector.h"
#include "../include/FileCrawler.h"
#include "../include/SearchEngine.h"

int main() {

    std::unordered_set<std::string> ignoreDirs = {};
    std::unordered_set<std::string> ignoreFiles = {};
    std::unordered_set<std::string> ignorePatterns = {};
    DatabaseConnector db1, db2;
    Indexer indexer = Indexer(&db1);
    FileCrawler crawler("D:\\Vs", ignoreDirs, ignoreFiles, ignorePatterns, &indexer);

    crawler.scanDirectory();

    SearchEngine searchEngine(&db2);

    char option;
    while (true) {
        std::cout << "Options:\n";
        std::cout << "s -> Search\n";
        std::cout << "c -> Crawling settings\n";
        std::cout << "q -> Quit\n";
        std::cin >> option;

        if (option == 's') {
            char searchOption;
            std::cout << "Search by content (y)?\n";
            std::cin >> searchOption;
            searchEngine.setSearchContent(searchOption == 'y');

            std::string searchTerm;
            std::cout << "Enter search term: ";
            std::cin.ignore();
            std::getline(std::cin, searchTerm);

            searchEngine.search(searchTerm);

        } else if (option == 'c') {
            char crawlOption;
            std::cout << "Crawling settings:\n";
            std::cout << "r -> Set root directory\n";
            std::cout << "d -> Add directories to ignore\n";
            std::cout << "f -> Add files to ignore\n";
            std::cout << "p -> Add patterns for file names to ignore\n";
            std::cout << "s -> Scan directories\n";
            std::cin >> crawlOption;

            if (crawlOption == 'r') {
                std::string rootDir;
                std::cout << "Enter root directory path: ";
                std::cin.ignore();
                std::getline(std::cin, rootDir);
                crawler.setRootDirectory(rootDir);
            } else if (crawlOption == 'd') {
                std::string dir;
                std::cout << "Enter directories to ignore (comma-separated): ";
                std::cin.ignore();
                std::getline(std::cin, dir);
                std::unordered_set<std::string> dirs;
                std::istringstream iss(dir);
                std::string path;
                while (std::getline(iss, path, ',')) {
                    dirs.insert(path);
                }
                crawler.setIgnoreDirectories(dirs);
            } else if (crawlOption == 'f') {
                std::string file;
                std::cout << "Enter files to ignore (comma-separated): ";
                std::cin.ignore();
                std::getline(std::cin, file);
                std::unordered_set<std::string> files;
                std::istringstream iss(file);
                std::string filePath;
                while (std::getline(iss, filePath, ',')) {
                    files.insert(filePath);
                }
                crawler.setIgnoreFiles(files);
            } else if (crawlOption == 'p'){
                std::string pattern;
                std::cout << "Enter patterns for which file names are ignored (comma-separated): ";
                std::cin.ignore();
                std::getline(std::cin, pattern);
                std::unordered_set<std::string> patterns;
                std::istringstream iss(pattern);
                std::string patternString;
                while (std::getline(iss, patternString, ',')) {
                    patterns.insert(patternString);
                }
                crawler.setIgnorePatterns(patterns);
            } else if (crawlOption == 's')
            {
                  crawler.scanDirectory();
            }
        } else if (option == 'q') {
            break;
        }
    }

    return 0;
}


