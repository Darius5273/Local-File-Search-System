#ifndef SEARCH_CONTROLLER_H
#define SEARCH_CONTROLLER_H

#include "httplib.h"
#include <memory>
#include "../include/SearchEngine.h"
#include "ISpellCorrectionStrategy.h"
#include "SearchResultAnalyzer.h"
#include "WidgetManager.h"

class SearchController {
public:
    explicit SearchController(ISearch& searchEngine, ISpellCorrectionStrategy* strategy);

    ISpellCorrectionStrategy* spellStrategy;
    void registerRoutes(httplib::Server& server);
    void setSpellCorrectionStrategy(ISpellCorrectionStrategy* strategy);

private:
    ISearch& searchEngine;
    QueryParser queryParser;
    SearchResultAnalyzer searchResultAnalyser;
    WidgetManager widgetManager;
    void handleSearch(const httplib::Request& req, httplib::Response& res);
};

#endif // SEARCH_CONTROLLER_H
