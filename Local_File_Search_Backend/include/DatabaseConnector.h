#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <pqxx/pqxx>
#include <vector>
#include "FileMetadata.h"
#include "SearchResult.h"

class DatabaseConnector {
private:
    std::string connectionString;
    pqxx::connection* conn;

public:
    DatabaseConnector(const std::string& connStr);
    ~DatabaseConnector();

    void connect();
    void disconnect();
    void insertBatch(const std::vector<FileMetadata>& files);
    std::vector<SearchResult> query(const std::string& searchTerm, bool searchContent);
};

#endif // DATABASE_CONNECTOR_H
