#ifndef SEARCH_CONTROLLER_H
#define SEARCH_CONTROLLER_H

#include "httplib.h"
#include <memory>
#include "../include/SearchEngine.h"

class SearchController {
public:
    explicit SearchController(SearchEngine& searchEngine);

    void registerRoutes(httplib::Server& server);

private:
    SearchEngine searchEngine;
    QueryParser queryParser;
    void handleSearch(const httplib::Request& req, httplib::Response& res);
};

#endif // SEARCH_CONTROLLER_H
