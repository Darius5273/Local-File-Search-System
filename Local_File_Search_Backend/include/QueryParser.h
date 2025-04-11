#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>

class QueryParser {
public:
    void parse(const std::string& query);
    const std::unordered_map<std::string, std::vector<std::string>>& getParsedQuery() const;

private:
    std::unordered_map<std::string, std::vector<std::string>> parsedQuery;

    void reset();
    void addQualifierValue(const std::string& qualifier, const std::string& value);
};

#endif // QUERY_PARSER_H
