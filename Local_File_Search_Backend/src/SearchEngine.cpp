#include "../include/SearchEngine.h"

SearchEngine::SearchEngine(DatabaseConnector* database)
        : db(database) {database->connect();}

void SearchEngine::setSearchContent(bool flag) {
    searchContent = flag;
}

void SearchEngine::search(const std::string& searchQuery) {
    queryParser.parse(searchQuery);
    auto parsedQuery= queryParser.getParsedQuery();
    std::vector<SearchResult> results = db->query(parsedQuery);
    std::cout << "Search Results:\n";
    for (const auto& result : results) {
        std::cout << "Path: " << result.path << "\n";
        std::cout << "Preview: " << result.previewText << "\n";
        std::cout << "\n";
    }
}
