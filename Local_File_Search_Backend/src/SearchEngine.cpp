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

void SearchEngine::search(const std::string& searchQuery) {
    queryParser.parse(searchQuery);
    auto parsedQuery= queryParser.getParsedQuery();
    observerContext.setParsedQuery(parsedQuery);

    std::vector<std::string> suggestions = suggestionObserver->getSuggestions();
    std::cout << "\nSuggestions:\n";
    for (const auto& suggestion : suggestions) {
        std::cout << "- " << suggestion << "\n";
    }

    std::vector<SearchResult> rankingResults = rankingObserver->getRankingResults();
    std::cout << "\nRanking Results:\n";
    for (auto it = rankingResults.rbegin(); it != rankingResults.rend(); ++it) {
        const auto& result = *it;
        std::cout << "Path: " << result.path << "\n";
        std::cout << "Score: " << result.score << "\n";
        std::cout << "Preview: " << result.previewText << "\n";
        std::cout << "\n";
    }
}

