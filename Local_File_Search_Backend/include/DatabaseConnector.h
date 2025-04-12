#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <pqxx/pqxx>
#include <vector>
#include "FileData.h"
#include "SearchResult.h"

class DatabaseConnector {
private:
    std::string connectionString;
    pqxx::connection* conn;

public:
    DatabaseConnector();
    ~DatabaseConnector();

    void connect();
    void disconnect();
    void insertBatch(const std::vector<FileData>& files);
    std::vector<SearchResult>query(const std::string& searchTerm, bool searchContent);
    std::vector<SearchResult> query(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);
    std::vector<SearchResult> DatabaseConnector::queryByPathAndColor(const std::vector<std::string>& pathTerms, const std::string& color);
    std::vector<SearchResult> DatabaseConnector::queryByContent(const std::vector<std::string>& contentTerms);

};



#endif // DATABASE_CONNECTOR_H
