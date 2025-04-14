#include "../include/DatabaseConnector.h"
#include "../include/RankingObserver.h"
#include <algorithm>
#include <queue>
#include <iostream>

RankingObserver::RankingObserver(DatabaseConnector* db) : db(db) {}

struct CompareSearchResultByScore {
    bool operator() (const SearchResult& a, const SearchResult& b) {
        return a.getScore() > b.getScore();
    }
};

const std::vector<SearchResult> RankingObserver::getRankingResults(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    auto results = db->query(parsedQuery);
    if (results.empty()) {
        rankingResults.clear();
        return rankingResults;
    }
    for (auto& result : results) {
        double score = rankingMap[result.getPath()].score;
        double newResult = result.getScore() + score;
        result.setScore(newResult);
    }

    std::priority_queue<SearchResult, std::vector<SearchResult>, CompareSearchResultByScore> minHeap;
    for (const auto& result : results) {
        minHeap.push(result);
        if (minHeap.size() > 3) {
            minHeap.pop();
        }
    }
    std::vector<std::string> historyEntry;
    rankingResults.clear();
    for (int i = 0; i < 3 && !minHeap.empty(); ++i) {
        auto result = minHeap.top();
        std::string path = result.getPath();
        minHeap.pop();
        rankingResults.push_back(result);
        historyEntry.push_back(path);
    }
    recentHistory.push_back(historyEntry);
    std::reverse(rankingResults.begin(), rankingResults.end());
    return rankingResults;
}

void RankingObserver::maintainRecentResults() {
    while (recentHistory.size() > 100) {
        auto oldResults = recentHistory.front();
        recentHistory.pop_front();

        for (int i = 0; i < oldResults.size(); ++i) {
            auto &info = rankingMap[oldResults[i]];
            if (i == 0) info.third--;
            else if (i == 1) info.second--;
            else if (i == 2) info.first--;

            if (info.first == 0 && info.second == 0 && info.third == 0) {
                rankingMap.erase(oldResults[i]);
            } else {
                info.score = (1.0 * info.first + 0.5 * info.second + 0.2 * info.third) / 100.0;
            }
        }
    }
}

void RankingObserver::update()  {
    if (recentHistory.empty()) return;
    auto lastResults = recentHistory.back();
    for (int i = 0; i < lastResults.size(); i++)
    {
        auto &info = rankingMap[lastResults[i]];
        if (i == 0) info.third++;
        else if (i == 1) info.second++;
        else if (i == 2) info.first++;

        info.score = (1.0 * info.first + 0.5 * info.second + 0.2 * info.third) / 100.0;
    }
    maintainRecentResults();

}
