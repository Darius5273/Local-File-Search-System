#include <gtest/gtest.h>
#include "../include/IndexScorer.h"

TEST(IndexScorerTest, PathDepthScore) {
    IndexScorer scorer;
    EXPECT_DOUBLE_EQ(scorer.pathDepthScore("C:/a/b"), 1.0);
    EXPECT_DOUBLE_EQ(scorer.pathDepthScore("C:/a/b/c/d"), 0.6);
    EXPECT_DOUBLE_EQ(scorer.pathDepthScore("C:/a/b/c/d/e/f/g"), 0.3);
    EXPECT_DOUBLE_EQ(scorer.pathDepthScore("C:/a/b/c/d/e/f/g/h/i/j/k"), 0.1);
}

TEST(IndexScorerTest, PathLengthScore) {
    IndexScorer scorer;
    EXPECT_DOUBLE_EQ(scorer.pathLengthScore(std::string(40, 'a')), 1.0);
    EXPECT_DOUBLE_EQ(scorer.pathLengthScore(std::string(60, 'a')), 0.7);
    EXPECT_DOUBLE_EQ(scorer.pathLengthScore(std::string(150, 'a')), 0.4);
    EXPECT_DOUBLE_EQ(scorer.pathLengthScore(std::string(300, 'a')), 0.1);
}

TEST(IndexScorerTest, MimeTypeScore) {
    IndexScorer scorer;
    EXPECT_DOUBLE_EQ(scorer.mimeTypeScore("application/pdf", ".pdf"), 0.9);
    EXPECT_DOUBLE_EQ(scorer.mimeTypeScore("unknown/type", ".txt"), 0.6);
    EXPECT_DOUBLE_EQ(scorer.mimeTypeScore("unknown/type", ".exe"), 0.1);
}

TEST(IndexScorerTest, SizeScore) {
    IndexScorer scorer;
    EXPECT_DOUBLE_EQ(scorer.sizeScore(100 * 1024), 0.8);       // 100 KB
    EXPECT_DOUBLE_EQ(scorer.sizeScore(700 * 1024), 0.4);       // 700 KB
    EXPECT_DOUBLE_EQ(scorer.sizeScore(5 * 1024 * 1024), 0.2);  // 5 MB
    EXPECT_DOUBLE_EQ(scorer.sizeScore(2LL * 1024 * 1024 * 1024), 0.1); // 2 GB
}

TEST(IndexScorerTest, RecencyScore) {
    IndexScorer scorer;

    // generate dates
    auto now = std::time(nullptr);
    auto tm_now = *std::localtime(&now);
    char buf[20];

    // less than 3 days ago
    tm_now.tm_mday -= 1;
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_now);
    EXPECT_DOUBLE_EQ(scorer.recencyScore(buf), 1.2);

    // ~10 days ago
    tm_now = *std::localtime(&now);
    tm_now.tm_mday -= 10;
    std::mktime(&tm_now);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_now);
    EXPECT_DOUBLE_EQ(scorer.recencyScore(buf), 0.8);
}

TEST(IndexScorerTest, TextScore) {
    IndexScorer scorer;
    EXPECT_DOUBLE_EQ(scorer.textScore(true), 1.0);
    EXPECT_DOUBLE_EQ(scorer.textScore(false), 0.6);
}

TEST(IndexScorerTest, ScoreFile_Composite) {
    IndexScorer scorer;
    std::string path = "C:/user/docs/file.txt";
    std::string ext = ".txt";
    std::string mime = "text/plain";

    std::time_t now = std::time(nullptr);
    std::tm tm_now = *std::localtime(&now);
    tm_now.tm_mday -= 2;
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_now);

    double result = scorer.scoreFile(path, ext, mime, buf, 400 * 1024, true);
    EXPECT_GT(result, 3.0);
}
