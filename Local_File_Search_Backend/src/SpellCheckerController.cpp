#include <nlohmann/json.hpp>
#include "../include/SpellCheckerController.h"

using json = nlohmann::json;

SpellCheckerController::SpellCheckerController(SearchController& searchController,
                                                       ISpellCorrectionStrategy* englishSpellChecker,
                                                       ISpellCorrectionStrategy* noSpellChecker)
        : searchController(searchController),
          englishSpell(englishSpellChecker),
          noSpell(noSpellChecker) {}


std::string SpellCheckerController::getCurrentSpellStrategy() const {
    return currentStrategyName;
}

void SpellCheckerController::registerRoutes(httplib::Server& server) {
    server.Post("/spell", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            currentStrategyName = body.value("spellStrategy", "noSpellCheck");
            ISpellCorrectionStrategy* strategy = nullptr;

            if (currentStrategyName == "englishSpellCheck") {
                strategy = englishSpell.get();
            } else {
                strategy = noSpell.get();
            }

            searchController.setSpellCorrectionStrategy(strategy);
            res.status = 200;
        } catch (...) {
            res.status = 400;
            res.set_content(R"({"error": "Invalid JSON or internal error"})", "application/json");
        }
    });

    server.Get("/spell-strategy", [this](const httplib::Request&, httplib::Response& res) {
        try {
            std::string currentStrategy = currentStrategyName;
            json response = {{"strategy", currentStrategy}};
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (...) {
            res.status = 500;
            res.set_content(R"({"error": "Failed to retrieve strategy"})", "application/json");
        }
    });

}

