#ifndef RANKINGOBSERVER_H
#define RANKINGOBSERVER_H

#include "IObserver.h"
#include "SearchResult.h"
#include "DatabaseConnector.h"
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <memory>


struct RankingInfo {
    int first = 0;
    int second = 0;
    int third = 0;
    double score = 0.0;
};


class RankingObserver : public IObserver {
public:
    RankingObserver(DatabaseConnector* db);
    void update() override;
    const std::vector<SearchResult> RankingObserver::getRankingResults(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery);


private:
    void maintainRecentResults();

    DatabaseConnector* db;
    std::unordered_map<std::string, RankingInfo> rankingMap;
    std::deque<std::vector<std::string>> recentHistory;
    std::vector<SearchResult> rankingResults;
};

#endif // RANKINGOBSERVER_H
