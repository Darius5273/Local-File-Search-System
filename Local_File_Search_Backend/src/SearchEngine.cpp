#include "../include/SearchEngine.h"



SearchEngine::SearchEngine(DatabaseConnector* database)
        : db(database) {
    database->connect();
    suggestionObserver = new SuggestionObserver();
    rankingObserver = new RankingObserver(database);
    observerContext.attach(suggestionObserver);
    observerContext.attach(rankingObserver);
}

void SearchEngine::setSearchContent(bool flag) {
    searchContent = flag;
}

void SearchEngine::update() {
    observerContext.notify();
}

std::vector<std::string> SearchEngine::getSuggestions(std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    return suggestionObserver->getSuggestions(parsedQuery);
}

std::vector<SearchResult> SearchEngine::getRankingResults(std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    std::vector<SearchResult> rankingResults = rankingObserver->getRankingResults(parsedQuery);
    return rankingResults;
}