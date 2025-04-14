#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

class QueryParser {
public:
    void parse(const std::string& query);
    const std::unordered_map<std::string, std::vector<std::string>>& getParsedQuery() const;

    void reset();

private:
    std::unordered_map<std::string, std::vector<std::string>> parsedQuery;
    const std::set<std::string> validQualifiers = {"color", "path", "content"};


    void addQualifierValue(const std::string& qualifier, const std::string& value);
};

#endif // QUERY_PARSER_H
