#include "../include/SearchEngine.h"

SearchEngine::SearchEngine(DatabaseConnector* database)
        : db(database) {database->connect();}

void SearchEngine::setSearchContent(bool flag) {
    searchContent = flag;
}

void SearchEngine::search(const std::string& searchTerm) {
    std::vector<SearchResult> results = db->query(searchTerm, searchContent);
    std::cout << "Search Results:\n";
    for (const auto& result : results) {
        std::cout << "Path: " << result.path << "\n";
        if (searchContent) {
            std::cout << "Preview: " << result.previewText << "\n";
        }
        std::cout << "-------------------\n";
    }
}
