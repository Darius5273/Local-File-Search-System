#include <iostream>
#include <unordered_set>
#include "../include/DatabaseConnector.h"
#include "../include/FileCrawler.h"
#include "../include/SearchEngine.h"
#include "../include/MarkdownReportFormatter.h"
#include "../include/httplib.h"
#include <opencv2/core/utils/logger.hpp>

//int main() {
//    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
//    std::unordered_set<std::string> ignoreDirs = {};
//    std::unordered_set<std::string> ignoreFiles = {};
//    std::unordered_set<std::string> ignorePatterns = {};
//    DatabaseConnector db1, db2;
//    Indexer indexer = Indexer(&db1);
//    std::unique_ptr<MarkdownReportFormatter> markdownReportFormatter = std::make_unique<MarkdownReportFormatter>();
//    indexer.setReportFormatter(std::move(markdownReportFormatter));
//    FileCrawler crawler("D:\\Vs", ignoreDirs, ignoreFiles, ignorePatterns, &indexer);
//
//    //crawler.scanDirectory();
//
//    SearchEngine searchEngine(&db2);
//
//    char option;
//    while (true) {
//        std::cout << "Options:\n";
//        std::cout << "s -> Search\n";
//        std::cout << "c -> Crawling settings\n";
//        std::cout << "q -> Quit\n";
//        std::cin >> option;
//
//        if (option == 's') {
//            char searchOption;
////            std::cout << "Search by content (y)?\n";
////            std::cin >> searchOption;
////            searchEngine.setSearchContent(searchOption == 'y');
//
//            std::string searchTerm;
//            std::cout << "Enter search term: ";
//            std::cin.ignore();
//            std::getline(std::cin, searchTerm);
//
//            searchEngine.search(searchTerm);
//
//        } else if (option == 'c') {
//            char crawlOption;
//            std::cout << "Crawling settings:\n";
//            std::cout << "r -> Set root directory\n";
//            std::cout << "d -> Add directories to ignore\n";
//            std::cout << "f -> Add files to ignore\n";
//            std::cout << "p -> Add patterns for file names to ignore\n";
//            std::cout << "s -> Scan directories\n";
//            std::cin >> crawlOption;
//
//            if (crawlOption == 'r') {
//                std::string rootDir;
//                std::cout << "Enter root directory path: ";
//                std::cin.ignore();
//                std::getline(std::cin, rootDir);
//                crawler.setRootDirectory(rootDir);
//            } else if (crawlOption == 'd') {
//                std::string dir;
//                std::cout << "Enter directories to ignore (comma-separated): ";
//                std::cin.ignore();
//                std::getline(std::cin, dir);
//                std::unordered_set<std::string> dirs;
//                std::istringstream iss(dir);
//                std::string path;
//                while (std::getline(iss, path, ';')) {
//                    dirs.insert(path);
//                }
//                crawler.setIgnoreDirectories(dirs);
//            } else if (crawlOption == 'f') {
//                std::string file;
//                std::cout << "Enter files to ignore (comma-separated): ";
//                std::cin.ignore();
//                std::getline(std::cin, file);
//                std::unordered_set<std::string> files;
//                std::istringstream iss(file);
//                std::string filePath;
//                while (std::getline(iss, filePath, ';')) {
//                    files.insert(filePath);
//                }
//                crawler.setIgnoreFiles(files);
//            } else if (crawlOption == 'p'){
//                std::string pattern;
//                std::cout << "Enter patterns for which file names are ignored (comma-separated): ";
//                std::cin.ignore();
//                std::getline(std::cin, pattern);
//                std::unordered_set<std::string> patterns;
//                std::istringstream iss(pattern);
//                std::string patternString;
//                while (std::getline(iss, patternString, ';')) {
//                    patterns.insert(patternString);
//                }
//                crawler.setIgnorePatterns(patterns);
//            } else if (crawlOption == 's')
//            {
//                  crawler.scanDirectory();
//            }
//        } else if (option == 'q') {
//            break;
//        }
//    }
//
//    return 0;
//}
//
//
//
//
#include "../include/SearchController.h"
#include "../include/CrawlerController.h"
#include "../include/SearchEngine.h"
#include "../include/ImageController.h"
#include "../include/SearchEngineProxy.h"
#include "../include/DictionaryLoader.h"
#include "../include/EnglishSpellCorrectionStrategy.h"
#include "../include/NoSpellCorrectionStrategy.h"
#include "../include/SpellCheckerController.h"

int main() {
    httplib::Server server;

    auto wordFreq = loadWordFrequencies("big.txt");
    auto colorFreq = loadColorFrequencies();
    EnglishSpellCorrectionStrategy englishSpell(wordFreq, colorFreq);
    NoSpellCorrectionStrategy noSpell;
    const std::string& configFile = "../../config.txt";
    DatabaseConnector db(configFile);
    SearchEngine searchEngine(&db);
    SearchEngineProxy proxyEngine(searchEngine, 100);
    SearchController searchController(proxyEngine, &noSpell);
    SpellCheckerController spellCheckerController(searchController, &englishSpell, &noSpell);
    CrawlerController crawlerController(configFile);
    ImageController imageController;


    imageController.registerRoutes(server);
    searchController.registerRoutes(server);
    crawlerController.registerRoutes(server);
    spellCheckerController.registerRoutes(server);

    std::cout << "Server running at http://localhost:8080\n";
    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        if (req.method == "OPTIONS") return httplib::Server::HandlerResponse::Handled;
        return httplib::Server::HandlerResponse::Unhandled;
    });

    server.listen("0.0.0.0", 8080);
}
