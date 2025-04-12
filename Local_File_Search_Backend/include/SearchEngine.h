#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <string>
#include <vector>
#include <iostream>
#include "DatabaseConnector.h"
#include "QueryParser.h"
#include "ObserverContext.h"
#include "../include/SuggestionObserver.h"
#include "../include/RankingObserver.h"

class SearchEngine {
private:
    DatabaseConnector* db;
    QueryParser queryParser;
    ObserverContext observerContext;
    SuggestionObserver* suggestionObserver;
    RankingObserver* rankingObserver;
    bool searchContent;

public:
    SearchEngine(DatabaseConnector* database);

    void setSearchContent(bool flag);

    void search(const std::string& searchTerm);
};

#endif // SEARCH_ENGINE_H
