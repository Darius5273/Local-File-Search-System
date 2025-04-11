#include "../include/DatabaseConnector.h"
#include "../include/RankingObserver.h"
#include <algorithm>
#include <queue>

RankingObserver::RankingObserver(DatabaseConnector* db) : db(db) {}

struct CompareSearchResultByScore {
    bool operator() (const SearchResult& a, const SearchResult& b) {
        return a.getScore() < b.getScore();
    }
};

void RankingObserver::update(const std::unordered_map<std::string, std::vector<std::string>>& parsedQuery) {
    auto results = db->query(parsedQuery);
    for (auto& result : results) {
        double score = rankingMap[result.path].score;
        double newResult = result.getScore() + score;
        result.setScore(newResult);
    }

    std::priority_queue<SearchResult, std::vector<SearchResult>, CompareSearchResultByScore> maxHeap;
    for (const auto& result : results) {
        maxHeap.push(result);
        if (maxHeap.size() > 3) {
            maxHeap.pop();
        }
    }
    std::vector<std::string> historyEntry;
    rankingResults.clear();
    for (int i = 0; i < maxHeap.size(); ++i) {
        auto result = maxHeap.top();
        std::string path = result.getPath();
        maxHeap.pop();
        rankingResults.push_back(result);
        auto &info = rankingMap[path];
        if (i == 1) info.first++;
        else if (i == 2) info.second++;
        else if (i == 3) info.third++;

        info.score = (1.0 * info.first + 0.5 * info.second + 0.2 * info.third) / 100.0;
        historyEntry.push_back(path);
    }
    recentHistory.push_back(historyEntry);
    maintainRecentResults();
}

void RankingObserver::maintainRecentResults() {
    while (recentHistory.size() > 100) {
        auto oldResults = recentHistory.front();
        recentHistory.pop_front();

        for (int i = 0; i < oldResults.size(); ++i) {
            auto &info = rankingMap[oldResults[i]];
            if (i == 0) info.first--;
            else if (i == 1) info.second--;
            else if (i == 2) info.third--;

            if (info.first == 0 && info.second == 0 && info.third == 0) {
                rankingMap.erase(oldResults[i]);
            } else {
                info.score = (1.0 * info.first + 0.5 * info.second + 0.2 * info.third) / 100.0;
            }
        }
    }
}

const std::vector<SearchResult> &RankingObserver::getRankingResults() const {
    return rankingResults;
}
