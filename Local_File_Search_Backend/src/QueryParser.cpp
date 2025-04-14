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
    bool colorQualifierSeen = false;

    while (stream >> token) {
        auto delimiterPos = token.find(':');
        if (delimiterPos != std::string::npos && delimiterPos != token.length() - 1) {
            std::string qualifier = token.substr(0, delimiterPos);
            std::string value = token.substr(delimiterPos + 1);

            if (validQualifiers.find(qualifier) == validQualifiers.end()) {
                throw std::runtime_error("Invalid qualifier");
            }

            std::transform(qualifier.begin(), qualifier.end(), qualifier.begin(), ::tolower);
            if (qualifier == "color") {
                if (colorQualifierSeen) {
                    throw std::runtime_error("Only one 'color' qualifier is allowed");
                }
                colorQualifierSeen = true;
            }
            if ((qualifier == "content" || qualifier == "path") && value.front() == '"') {
                value.erase(0, 1);
                std::string word;
                bool closed = false;

                while (stream >> word) {
                    auto quotePos = word.find('"');
                    if (quotePos != std::string::npos) {
                        value += " " + word.substr(0, quotePos);
                        closed = true;
                        break;
                    } else {
                        value += " " + word;
                    }
                }

                if (!closed) {
                    throw std::runtime_error("Unclosed quote in content/path qualifier");
                }
            }

            addQualifierValue(qualifier, value);
        }
        else {
            throw std::runtime_error("Invalid query");
        }

    }

    if (parsedQuery.empty())
        throw std::runtime_error("Invalid query");
}

const std::unordered_map<std::string, std::vector<std::string>>& QueryParser::getParsedQuery() const {
    return parsedQuery;
}
