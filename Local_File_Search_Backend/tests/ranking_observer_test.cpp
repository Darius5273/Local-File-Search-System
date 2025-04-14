#include <gtest/gtest.h>
#include "../include/RankingObserver.h"
#include <pqxx/pqxx>
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")


SearchResult makeResult(const std::string& path, double score, const std::string& preview = "") {
    return SearchResult(path, score, preview);
}

std::string generateUUID() {
    UUID uuid;
    UuidCreate(&uuid);

    RPC_CSTR str;
    UuidToStringA(&uuid, &str);

    std::string uuidStr(reinterpret_cast<char*>(str));
    RpcStringFreeA(&str);

    return uuidStr;
}
void insertTestRow(pqxx::connection& conn, const std::string& id, const std::string& name,
                   const std::string& path, double score, const std::string& content,
                   const std::string& mime_type, const std::string& extension = ".txt", long long size = 1000) {
    pqxx::work txn(conn);
    txn.exec0("INSERT INTO files (id, name, path, score) VALUES ('" + id + "', '" + name + "', '" + path + "', " + std::to_string(score) + ")");
    txn.exec0("INSERT INTO metadata (id, size, extension, mime_type, modified_time) "
              "VALUES ('" + id + "', " + std::to_string(size) + ", '" + extension + "', '" + mime_type + "', NOW())");
    txn.exec0("INSERT INTO textual_content (file_id, content) VALUES ('" + id + "', '" + txn.esc(content) + "')");
    txn.commit();
}

void deleteTestRow(pqxx::connection& conn, const std::string& id) {
    pqxx::work txn(conn);
    txn.exec0("DELETE FROM textual_content WHERE file_id = '" + id + "'");
    txn.exec0("DELETE FROM metadata WHERE id = '" + id + "'");
    txn.exec0("DELETE FROM files WHERE id = '" + id + "'");
    txn.commit();
}

TEST(RankingObserverTest, RealDB_Top3FilesByScore) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::cout<<current_path.string();
    std::filesystem::path config_path = current_path  /"test_config.txt";
    DatabaseConnector db(config_path.string());

    std::vector<std::tuple<std::string, std::string, double>> testFiles = {
            {"file1.txt", "file1 content preview", 1.0},
            {"file2.txt", "file2 content preview", 2.0},
            {"file3.txt", "file3 content preview", 3.0},
            {"file4.txt", "file4 content preview", 4.0}
    };

    std::vector<std::string> ids;
    for (const auto& [name, content, score] : testFiles) {
        std::string id = generateUUID();
        ids.push_back(id);
        db.connect();
        insertTestRow(*db.getConn(), id, name, "/tmp/" + name, score, content, "text/plain");
    }

    RankingObserver observer(&db);
    std::unordered_map<std::string,std::vector<std::string>> parsedQuery;
    parsedQuery["path"].emplace_back("tmp");
    auto results = observer.getRankingResults(parsedQuery);

    ASSERT_EQ(results.size(), 3);

    EXPECT_EQ(results[0].getPath(), "/tmp/file4.txt");
    EXPECT_EQ(results[1].getPath(), "/tmp/file3.txt");
    EXPECT_EQ(results[2].getPath(), "/tmp/file2.txt");

    EXPECT_NEAR(results[0].getScore(), 4.0, 0.001);
    EXPECT_NEAR(results[1].getScore(), 3.0, 0.001);
    EXPECT_NEAR(results[2].getScore(), 2.0, 0.001);

    observer.update();

    results = observer.getRankingResults(parsedQuery);

    EXPECT_NEAR(results[0].getScore(), 4.0 + 1/100.0, 0.0001);
    EXPECT_NEAR(results[1].getScore(), 3.0 + 0.5/100.0, 0.0001);
    EXPECT_NEAR(results[2].getScore(), 2.0 + 0.2/100.0, 0.0001);

    for (const auto& id : ids) {
        deleteTestRow(*db.getConn(), id);
    }
}
