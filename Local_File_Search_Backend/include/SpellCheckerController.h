
#ifndef SPELLCHECKERCONTROLLER_H
#define SPELLCHECKERCONTROLLER_H
#pragma once
#include "SearchController.h"
#include <memory>

class SpellCheckerController {
public:
    SpellCheckerController(SearchController& searchController,
                               ISpellCorrectionStrategy* englishSpellChecker,
                               ISpellCorrectionStrategy* none);

    void registerRoutes(httplib::Server& server);
    std::string getCurrentSpellStrategy() const;

private:
    SearchController& searchController;
    std::unique_ptr<ISpellCorrectionStrategy> englishSpell;
    std::unique_ptr<ISpellCorrectionStrategy> noSpell;
    std::string currentStrategyName = "noSpellCheck";
};

#endif //SPELLCHECKERCONTROLLER_H
