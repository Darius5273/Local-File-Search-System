#include "../include/QueryParser.h"
#include <sstream>
#include <algorithm>

void QueryParser::reset() {
    parsedQuery.clear();
}

void QueryParser::addQualifierValue(const std::string& qualifier, const std::string& value) {
    parsedQuery[qualifier].push_back(value);
}

void QueryParser::parse(const std::string& query) {
    reset();

    std::istringstream stream(query);
    std::string token;

    while (stream >> token) {
        auto delimiterPos = token.find(':');
        if (delimiterPos != std::string::npos && delimiterPos != token.length() - 1) {
            std::string qualifier = token.substr(0, delimiterPos);
            std::string value = token.substr(delimiterPos + 1);

            std::transform(qualifier.begin(), qualifier.end(), qualifier.begin(), ::tolower);

            addQualifierValue(qualifier, value);
        }
    }
}

const std::unordered_map<std::string, std::vector<std::string>>& QueryParser::getParsedQuery() const {
    return parsedQuery;
}
