#include <gtest/gtest.h>
#include "../include/QueryParser.h"

TEST(QueryParserTest, ValidSingleQualifier) {
    QueryParser parser;
    parser.parse("color:red");

    auto result = parser.getParsedQuery();
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result.at("color")[0], "red");
}

TEST(QueryParserTest, ValidMultipleQualifiers) {
    QueryParser parser;
    parser.parse("color:blue path:C:/Users/ content:sample");

    auto result = parser.getParsedQuery();
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result.at("color")[0], "blue");
    EXPECT_EQ(result.at("path")[0], "C:/Users/");
    EXPECT_EQ(result.at("content")[0], "sample");
}

TEST(QueryParserTest, QuotedContentWithSpaces) {
    QueryParser parser;
    parser.parse("content:\"hello world test\"");

    auto result = parser.getParsedQuery();
    ASSERT_TRUE(result.find("content") != result.end());
    EXPECT_EQ(result.at("content")[0], "hello world test");
}

TEST(QueryParserTest, QuotedPathWithSpaces) {
    QueryParser parser;
    parser.parse("path:\"C:/My Folder/file.txt\"");

    auto result = parser.getParsedQuery();
    EXPECT_EQ(result.at("path")[0], "C:/My Folder/file.txt");
}

TEST(QueryParserTest, InvalidQualifierThrows) {
    QueryParser parser;
    EXPECT_THROW(parser.parse("type:pdf"), std::runtime_error);
}

TEST(QueryParserTest, DuplicateColorQualifierThrows) {
    QueryParser parser;
    EXPECT_THROW(parser.parse("color:red color:blue"), std::runtime_error);
}

TEST(QueryParserTest, UnclosedQuoteThrows) {
    QueryParser parser;
    EXPECT_THROW(parser.parse("content:\"open text without end"), std::runtime_error);
}

TEST(QueryParserTest, MissingValueThrows) {
    QueryParser parser;
    EXPECT_THROW(parser.parse("content:"), std::runtime_error);
}

TEST(QueryParserTest, NoValidTokensThrows) {
    QueryParser parser;
    EXPECT_THROW(parser.parse("justtextwithoutcolon"), std::runtime_error);
}

TEST(QueryParserTest, ResetClearsParsedQuery) {
    QueryParser parser;
    parser.parse("color:blue");
    parser.reset();
    EXPECT_TRUE(parser.getParsedQuery().empty());
}
