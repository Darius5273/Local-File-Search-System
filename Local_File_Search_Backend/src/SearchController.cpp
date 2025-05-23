#include "../include/SearchController.h"
#include "../include/ImageGalleryWidget.h"
#include "../include/ProgrammingLanguageWidget.h"
#include "../include/WidgetFactory.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SearchController::SearchController(ISearch& engine, ISpellCorrectionStrategy* strategy)
        : searchEngine(engine), spellStrategy(strategy) {
    widgetManager.registerObserver(std::make_shared<ImageGalleryWidget>());
    widgetManager.registerObserver(std::make_shared<ProgrammingLanguageWidget>());
}
void SearchController::registerRoutes(httplib::Server& server) {
    server.Post("/search", [this](const httplib::Request& req, httplib::Response& res) {
        handleSearch(req, res);
    });
}

void SearchController::setSpellCorrectionStrategy(ISpellCorrectionStrategy* strategy) {
    spellStrategy = strategy;
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

        std::string correctedQueryString;
        auto correctedParsedQuery = spellStrategy->correct(parsedQuery, correctedQueryString);
        auto bundle = searchEngine.search(correctedParsedQuery);
        json response = searchResultAnalyser.analyzeMetadata(bundle.rankingResults);

        WidgetFactory widgetFactory;
        auto keywordWidgets = widgetFactory.createWidgets(correctedParsedQuery);
        json keywordWidgetsJson = json::array();
        for (const auto& w : keywordWidgets) {
            keywordWidgetsJson.push_back(w->toJson());
        }
        response["keywordWidgets"] = keywordWidgetsJson;

        widgetManager.updateAll(correctedParsedQuery, bundle.rankingResults);
        std::string contextWidgets;
        auto activeContextWidgets = widgetManager.getActiveWidgets();
        for (const auto& w : activeContextWidgets)
        {
            contextWidgets += w + "//";
        }
        response["contextWidgets"] = contextWidgets;

        response["correctedQuery"] = correctedQueryString;
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
