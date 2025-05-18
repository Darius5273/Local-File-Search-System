#include <nlohmann/json.hpp>
#include "../include/SpellCheckerController.h"

using json = nlohmann::json;

SpellCheckerController::SpellCheckerController(SearchController& searchController,
                                                       ISpellCorrectionStrategy* englishSpellChecker,
                                                       ISpellCorrectionStrategy* noSpellChecker)
        : searchController(searchController),
          englishSpell(englishSpellChecker),
          noSpell(noSpellChecker) {}

void SpellCheckerController::registerRoutes(httplib::Server& server) {
    server.Post("/spell", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            std::string spellStrategyName = body.value("spellStrategy", "noSpellCheck");
            ISpellCorrectionStrategy* strategy = nullptr;

            if (spellStrategyName == "englishSpellCheck") {
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
}
