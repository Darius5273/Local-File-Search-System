#include <thread>
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

QueryResultBundle SearchEngine::search(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    QueryResultBundle bundle;
    bundle.suggestions = getSuggestions(const_cast<std::unordered_map<std::string, std::vector<std::string>>&>(parsedQuery));
    bundle.rankingResults = getRankingResults(const_cast<std::unordered_map<std::string, std::vector<std::string>>&>(parsedQuery));

    std::thread([this]() {
        this->update();
    }).detach();

    return bundle;
}