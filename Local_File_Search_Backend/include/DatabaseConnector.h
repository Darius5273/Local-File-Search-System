#ifndef DATABASE_CONNECTOR_H
#define DATABASE_CONNECTOR_H

#include <pqxx/pqxx>
#include <vector>
#include "FileData.h"
#include "SearchResult.h"

class DatabaseConnector {
private:
    std::string connectionString = "host=localhost port=5432 dbname=local_files user=postgres password=tresspress79";
    pqxx::connection* conn;

public:
    DatabaseConnector();
    ~DatabaseConnector();

    void connect();
    void disconnect();
    void insertBatch(const std::vector<FileData>& files);
    std::vector<SearchResult>query(const std::string& searchTerm, bool searchContent);

};



#endif // DATABASE_CONNECTOR_H
