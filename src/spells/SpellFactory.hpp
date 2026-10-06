#pragma once

#include "Spell.hpp"
#include <memory>
#include <string>

enum class SpellType {
    FIRE,
    HEALING,
    SHIELD,
    DARK_ATTACK,
    BUBBLE_WAVE,
    MERGE_MAGIC,
    QUICK_STRIKE
};

class SpellFactory {
public:
    static std::unique_ptr<Spell> createSpell(SpellType type);
    static std::unique_ptr<Spell> createSpellByName(const std::string& name);
};
