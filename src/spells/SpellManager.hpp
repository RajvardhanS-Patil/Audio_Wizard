#pragma once

#include "Spell.hpp"
#include <vector>
#include <memory>

class SpellManager {
public:
    SpellManager();

    void registerSpell(std::unique_ptr<Spell> spell);
    void update(float deltaTime);

    Spell* findMatchingSpell(const std::vector<NotePitch>& sequence);
    const std::vector<std::unique_ptr<Spell>>& getAllSpells() const { return m_spells; }
    Spell* getSpellByName(const std::string& name);

private:
    std::vector<std::unique_ptr<Spell>> m_spells;
};
