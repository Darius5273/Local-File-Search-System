#include <iostream>
#include <unordered_set>
#include "../include/DatabaseConnector.h"
#include "../include/FileCrawler.h"
#include "../include/SearchEngine.h"
#include "../include/MarkdownReportFormatter.h"
#include "../include/httplib.h"
#include <opencv2/core/utils/logger.hpp>
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
