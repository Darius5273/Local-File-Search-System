#include "../include/SearchController.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SearchController::SearchController(ISearch& searchEngine) : searchEngine(searchEngine) {}

void SearchController::registerRoutes(httplib::Server& server) {
    server.Post("/search", [this](const httplib::Request& req, httplib::Response& res) {
        handleSearch(req, res);
    });
}

void SearchController::handleSearch(const httplib::Request& req, httplib::Response& res) {
    try {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            res.status = 400;
            res.set_content(R"({"error": "Invalid JSON format"})", "application/json");
            return;
        }

        if (!body.contains("query") || !body["query"].is_string()) {
            res.status = 400;
            res.set_content(R"({"error": "Missing or invalid 'query' field"})", "application/json");
            return;
        }

        std::string query = body["query"];
        queryParser.parse(query);
        auto parsedQuery= queryParser.getParsedQuery();

        auto bundle = searchEngine.search(parsedQuery);
        json response;
        response["suggestions"] = bundle.suggestions;
        for (const auto& result : bundle.rankingResults) {
            response["rankingResults"].push_back({
                                                         {"path", result.path},
                                                         {"score", result.score},
                                                         {"preview", result.previewText},
                                                         {"is_image", result.is_image}
                                                 });
        }
        res.set_content(response.dump(), "application/json");
    }
    catch (const std::runtime_error& e) {
        res.status = 400;
        json err = { {"error", e.what()} };
        res.set_content(err.dump(), "application/json");
    }
    catch (...) {
        res.status = 500;
        res.set_content(R"({"error": "Internal server error"})", "application/json");
    }
}
