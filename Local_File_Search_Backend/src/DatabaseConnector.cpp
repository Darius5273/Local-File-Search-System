#include "../include/DatabaseConnector.h"
#include "../include/Utf8Converter.h"
#include "../include/TextExtractor.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
const long long CONTENT_BATCH_SIZE = 8 * 1024 * 1024;
DatabaseConnector::DatabaseConnector()
        : conn(nullptr) {
    std::ifstream configFile("../../config.txt");
    std::getline(configFile, connectionString);
}

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
        std::string filesValues;
        for (size_t i = 0; i < files.size(); ++i) {
            const auto& file = files[i];
            filesValues += "(" + txn.quote(file.id) + ", "
                           + txn.quote(file.name) + ", "
                           + txn.quote(file.path) + ")";
            if (i != files.size() - 1) {
                filesValues += ", ";
            }
        }
        filesQuery += filesValues;
        filesQuery += " ON CONFLICT (path) DO UPDATE SET name = EXCLUDED.name RETURNING id, path;";

        std::unordered_map<std::string, std::string> pathToIdMap;

        pqxx::result fileResult = txn.exec(filesQuery);
        for (const auto& row : fileResult) {
            std::string returnedId = row["id"].as<std::string>();
            std::string returnedPath = row["path"].as<std::string>();
            pathToIdMap[returnedPath] = returnedId;
        }

        std::string metadataQuery = "INSERT INTO metadata (id, size, extension, mime_type, modified_time) VALUES ";
        std::string textualContentQuery = "INSERT INTO textual_content (file_id, content) VALUES ";
        bool executeTextualContentQuery = false;
        long long currentSize = 0;
        std::string metadataValues;
        for (size_t i = 0; i < files.size(); ++i) {
            const auto& file = files[i];
            const auto& correctId = pathToIdMap[file.path];
            metadataValues += "(" + txn.quote(correctId) + ", "
                              + std::to_string(file.size) + ", "
                              + txn.quote(file.extension) + ", "
                              + txn.quote(file.mime_type) + ", "
                              + txn.quote(file.modified_time) + ")";
            if (i != files.size() - 1) {
                metadataValues += ", ";
            }
            if (file.is_text) {
                currentSize += file.size;
                if(CONTENT_BATCH_SIZE <= currentSize && executeTextualContentQuery)
                {
                    textualContentQuery += "ON CONFLICT (file_id) DO UPDATE SET content = EXCLUDED.content;";
                    txn.exec(textualContentQuery);
                    textualContentQuery = "INSERT INTO textual_content (file_id, content) VALUES ";
                    executeTextualContentQuery = false;
                    currentSize = 0;
                }
                if(executeTextualContentQuery)
                    textualContentQuery += ", ";
                executeTextualContentQuery = true;

                try {
                    std::string fileContent = TextExtractor::GetFileContent(files[i]);
                    textualContentQuery += "(" + txn.quote(correctId) + ", " + txn.quote(fileContent) + ") ";

                } catch (const std::exception& e) {
                    std::cerr << "Error processing file " << file.path << ": " << e.what() << "\n";
                    continue;
                }
            }
        }
        metadataQuery += metadataValues;
        metadataQuery += " ON CONFLICT (id) DO UPDATE SET size = EXCLUDED.size, extension = EXCLUDED.extension, mime_type = EXCLUDED.mime_type, modified_time = EXCLUDED.modified_time;";
        textualContentQuery += "ON CONFLICT (file_id) DO UPDATE SET content = EXCLUDED.content;";
        txn.exec(metadataQuery);

        if (executeTextualContentQuery) {
            txn.exec(textualContentQuery);
        }

        txn.commit();

    } catch (const std::exception& e) {
        std::cerr << "Batch insert error: " << e.what() << std::endl;
    }
}

std::vector<SearchResult> DatabaseConnector::queryByPath(const std::vector<std::string>& pathTerms) {
    std::vector<SearchResult> results;
    if (pathTerms.empty()) return results;

    try {
        pqxx::work txn(*conn);
        std::string sql = R"SQL(
            SELECT
                f.path, f.score,
                substring(tc.content FROM '(^(\\S+\\s+){100}(\\S+))') AS preview
            FROM files f
            JOIN textual_content tc ON f.id = tc.file_id
            WHERE
        )SQL";

        std::vector<std::string> params;
        for (size_t i = 0; i < pathTerms.size(); ++i) {
            if (i > 0) sql += " AND ";
            sql += "LOWER(f.path) ILIKE LOWER($" + std::to_string(i + 1) + ")";
            params.push_back("%" + pathTerms[i] + "%");
        }

        sql += " ORDER BY f.score DESC LIMIT 3;";

        pqxx::result res = txn.exec_params(sql, params);

        for (const auto& row : res) {
            std::string path = row["path"].c_str();
            std::string preview = row["preview"].c_str();
            results.emplace_back(path, preview);
        }

    } catch (const std::exception& e) {
        std::cerr << "QueryByPath error: " << e.what() << std::endl;
    }

    return results;
}

std::vector<SearchResult> DatabaseConnector::queryByContent(const std::vector<std::string>& contentTerms) {
    std::vector<SearchResult> results;
    if (contentTerms.empty()) return results;

    try {
        pqxx::work txn(*conn);

        std::string tsqueryStr;
        for (const auto& term : contentTerms) {
            if (!tsqueryStr.empty()) tsqueryStr += " & ";
            tsqueryStr += term + ":*";
        }

        std::string sql = R"SQL(
            SELECT
                f.path, f.score,
                ts_headline('simple', tc.content, query, 'MaxFragments=1, MaxWords=100') AS preview
            FROM (
                SELECT
                    file_id,
                    content,
                    to_tsquery('simple', $1) AS query
                FROM textual_content
                WHERE to_tsvector('simple', content) @@ to_tsquery('simple', $1)
            ) tc
            JOIN files f ON f.id = tc.file_id
            ORDER BY f.score DESC
            LIMIT 3;
        )SQL";

        pqxx::result res = txn.exec_params(sql, tsqueryStr);

        for (const auto& row : res) {
            std::string path = row["path"].c_str();
            std::string preview = row["preview"].c_str();
            results.emplace_back(path, preview);
        }

    } catch (const std::exception& e) {
        std::cerr << "QueryByContent error: " << e.what() << std::endl;
    }

    return results;
}


std::vector<SearchResult> DatabaseConnector::query(const std::string& searchTerm, bool searchContent) {
    std::vector<SearchResult> results;
    try {
        pqxx::work txn(*conn);
        std::string sql;
        //std::cout<<"*"<<std::endl;
        if (searchContent) {
            sql = "SELECT f.path, ts_headline('simple', fc.content, plainto_tsquery($1), 'MaxFragments=1, MaxWords=100') AS preview "
                  "FROM ("
                  "SELECT file_id, content"
                  "    FROM textual_content"
                  "    WHERE to_tsvector('simple', content) @@ plainto_tsquery($1 || ':*')"
                  "    LIMIT 3 "
                  ") fc "
                  "JOIN files f ON f.id = fc.file_id;";
        } else {
            sql = "SELECT f.path, substring(tc.content FROM '(^(\\S+\\s+){100}(\\S+))') AS preview "
                  "FROM files f JOIN textual_content tc ON f.id = tc.file_id "
                  "WHERE LOWER(name) LIKE LOWER('%' || $1 || '%') "
                  "LIMIT 3;";
        }

        pqxx::result res = txn.exec_params(sql, searchTerm);

        for (auto row : res) {
            std::string path = row["path"].c_str();
            std::string preview = row["preview"].c_str();
            results.emplace_back(path, preview);
        }
    } catch (const std::exception& e) {
        std::cerr << "Query error: " << e.what() << std::endl;
    }

    return results;
}

std::vector<SearchResult> DatabaseConnector::query(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    std::vector<SearchResult> results;

    try {
        pqxx::work txn(*conn);

        std::vector<std::string> contentTerms;
        std::vector<std::string> pathTerms;

        if (parsedQuery.count("content"))
            contentTerms = parsedQuery.at("content");
        if (parsedQuery.count("path"))
            pathTerms = parsedQuery.at("path");

        if (contentTerms.empty()) {
            return queryByPath(pathTerms);
        }
        if (pathTerms.empty()) {
            return queryByContent(contentTerms);
        }

        std::string sql;
        std::vector<std::string> params;

        std::string tsqueryStr;
        if (!contentTerms.empty()) {
            for (const auto& term : contentTerms) {
                if (!tsqueryStr.empty()) tsqueryStr += " & ";
                tsqueryStr += term + ":*";
            }
        }

        sql += "SQL("
               "SELECT "
               "f.path, f.score, ts_headline('simple', tc.content, query, 'MaxFragments=1, MaxWords=100') AS preview "
               "FROM ( "
               "SELECT file_id, content, to_tsquery('simple', $1) AS query "
               "FROM textual_content "
               "WHERE to_tsvector('simple', content) @@ to_tsquery('simple', $1) "
               ") tc "
               "JOIN files f ON f.id = tc.file_id)";

        params.push_back(tsqueryStr);
        int paramIndex = 2;

        if (!pathTerms.empty()) {
            sql += " WHERE ";
            for (size_t i = 0; i < pathTerms.size(); ++i) {
                if (i > 0) sql += " AND ";
                sql += "LOWER(f.path) ILIKE LOWER($" + std::to_string(paramIndex) + ")";
                params.push_back("%" + pathTerms[i] + "%");
                paramIndex++;
            }
        }

        sql += " ORDER BY f.score DESC LIMIT 3;";

        pqxx::result res = txn.exec_params(sql, params);

        for (const auto& row : res) {
            std::string path = row["path"].c_str();
            std::string preview = row["preview"].c_str();
            results.emplace_back(path, preview);
        }

    } catch (const std::exception& e) {
        std::cerr << "Query error: " << e.what() << std::endl;
    }

    return results;
}
