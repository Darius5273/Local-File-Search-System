#ifndef CRAWLER_CONTROLLER_H
#define CRAWLER_CONTROLLER_H

#include "httplib.h"
#include <string>
#include <unordered_set>
#include "FileCrawler.h"
#include "MarkdownReportFormatter.h"
#include "TextReportFormatter.h"

class CrawlerController {
public:
    CrawlerController(const std::string& file);
    void registerRoutes(httplib::Server& server);

private:
    void handleCrawl(const httplib::Request& req, httplib::Response& res);

    DatabaseConnector db;
};

#endif // CRAWLER_CONTROLLER_H
