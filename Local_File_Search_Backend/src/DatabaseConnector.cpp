#include "../include/DatabaseConnector.h"
#include <iostream>
#include <fstream>

DatabaseConnector::DatabaseConnector()
        : conn(nullptr) {}

DatabaseConnector::~DatabaseConnector() {
    disconnect();
}

void DatabaseConnector::connect() {
    try {
        conn = new pqxx::connection(connectionString);
        if (!conn->is_open()) {
            std::cerr << "Failed to connect to database." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

void DatabaseConnector::disconnect() {
    if (conn != nullptr) {
        delete conn;
        conn = nullptr;
    }
}

void DatabaseConnector::insertBatch(const std::vector<FileData>& files) {
    try {
        pqxx::work txn(*conn);

        std::string filesQuery = "INSERT INTO files (id, name, path) VALUES ";
        std::string metadataQuery = "INSERT INTO metadata (id, size, extension, mime_type, created_at, modified_time) VALUES ";
        std::string textualContentQuery = "INSERT INTO textual_content (file_id, content) VALUES ";
        bool executeTextualContentQuery = false;

        for (size_t i = 0; i < files.size(); ++i) {
            const FileData& file = files[i];
            filesQuery += "(" + txn.quote(file.id) + ", "
                          + txn.quote(file.name) + ", "
                          + txn.quote(file.path) + ")";

            metadataQuery += "(" + txn.quote(file.id) + ", "
                             + std::to_string(file.size) + ", "
                             + txn.quote(file.extension) + ", "
                             + txn.quote(file.mime_type) + ", "
                             + txn.quote(file.created_at) + ", "
                             + txn.quote(file.modified_time) + ")";

            if (file.is_text) {
                if(executeTextualContentQuery)
                    textualContentQuery += ", ";
                executeTextualContentQuery = true;
                std::ifstream fileStream(file.path);
                if (fileStream) {
                    std::ostringstream contentBuffer;
                    contentBuffer << fileStream.rdbuf();
                    textualContentQuery += "(" + txn.quote(file.id) + ", " + txn.quote(contentBuffer.str()) + ") ";
                }
            }

            if (i != files.size() - 1) {
                filesQuery += ", ";
                metadataQuery += ", ";
            }
        }

        filesQuery += " ON CONFLICT (path) DO UPDATE SET name = EXCLUDED.name;";
        metadataQuery += " ON CONFLICT (id) DO UPDATE SET size = EXCLUDED.size, extension = EXCLUDED.extension, mime_type = EXCLUDED.mime_type, created_at = EXCLUDED.created_at, modified_time = EXCLUDED.modified_time;";
        textualContentQuery += "ON CONFLICT (file_id) DO UPDATE SET content = EXCLUDED.content;";

        txn.exec(filesQuery);
        txn.exec(metadataQuery);
        if (executeTextualContentQuery) {
            txn.exec(textualContentQuery);
        }

        txn.commit();

    } catch (const std::exception& e) {
        std::cerr << "Insert batch error: " << e.what() << std::endl;
    }
}


std::vector<SearchResult> DatabaseConnector::query(const std::string& searchTerm, bool searchContent) {
    std::vector<SearchResult> results;
    try {
        pqxx::work txn(*conn);
        std::string sql;

        if (searchContent) {
            sql = "SELECT f.path, ts_headline('simple', fc.content_text, plainto_tsquery($1), 'MaxFragments=1, MaxWords=100') AS preview "
                  "FROM file_content fc JOIN files f ON f.id = fc.file_id "
                  "WHERE fc.content_text @@ plainto_tsquery($1);";
        } else {
            sql = "SELECT path FROM files WHERE LOWER(name) LIKE LOWER('%' || $1 || '%');";
        }

        pqxx::result res = txn.exec_params(sql, searchTerm);

        for (auto row : res) {
            std::string path = row["path"].c_str();
            std::string preview = searchContent ? row["preview"].c_str() : "";
            results.emplace_back(path, preview);
        }
    } catch (const std::exception& e) {
        std::cerr << "Query error: " << e.what() << std::endl;
    }

    return results;
}
