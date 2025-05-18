
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

private:
    SearchController& searchController;
    std::unique_ptr<ISpellCorrectionStrategy> englishSpell;
    std::unique_ptr<ISpellCorrectionStrategy> noSpell;
};

#endif //SPELLCHECKERCONTROLLER_H
