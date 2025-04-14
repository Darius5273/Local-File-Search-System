#include "../include/CrawlerController.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <sstream>

using json = nlohmann::json;

CrawlerController::CrawlerController(const std::string& file) : db(file) {}

void CrawlerController::registerRoutes(httplib::Server& server) {
    server.Post("/crawl", [this](const httplib::Request& req, httplib::Response& res) {
        handleCrawl(req, res);
    });
}

void CrawlerController::handleCrawl(const httplib::Request& req, httplib::Response& res) {
    try {
        json body = json::parse(req.body);

        if (!body.contains("rootPath") || !body["rootPath"].is_string()) {
            res.status = 400;
            res.set_content(R"({"error": "Missing 'rootPath'"})", "application/json");
            return;
        }

        std::string rootPath = body["rootPath"];
        std::unordered_set<std::string> ignoreDirs, ignoreFiles, ignorePatterns;

        for (const auto& item : body.value("ignoreDirs", json::array())) {
            ignoreDirs.insert(item.get<std::string>());
        }
        for (const auto& item : body.value("ignoreFiles", json::array())) {
            ignoreFiles.insert(item.get<std::string>());
        }
        for (const auto& item : body.value("ignorePatterns", json::array())) {
            ignorePatterns.insert(item.get<std::string>());
        }

        Indexer indexer(&db);

        std::string reportFormat = body.value("reportFormat", "markdown");
        if (reportFormat == "markdown") {
            indexer.setReportFormatter(std::make_unique<MarkdownReportFormatter>());
        } else if (reportFormat == "text") {
            indexer.setReportFormatter(std::make_unique<TextReportFormatter>());
        } else {
            res.status = 400;
            res.set_content(R"({"error": "Invalid 'reportFormat'. Use 'markdown' or 'text'"})", "application/json");
            return;
        }

        FileCrawler crawler(rootPath, ignoreDirs, ignoreFiles, ignorePatterns, &indexer);
        crawler.scanDirectory();

        json response;
        response["message"] = "Crawling completed successfully.";
        res.set_content(response.dump(), "application/json");
    }
    catch (const std::exception& e) {
        res.status = 500;
        json err = { {"error", e.what()} };
        res.set_content(err.dump(), "application/json");
    }
}
