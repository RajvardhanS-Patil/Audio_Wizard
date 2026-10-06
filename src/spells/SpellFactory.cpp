#include "SpellFactory.hpp"
#include "FireSpell.hpp"
#include "HealingSpell.hpp"
#include "ShieldSpell.hpp"
#include "DarkAttackSpell.hpp"
#include "SortingSpells.hpp"

std::unique_ptr<Spell> SpellFactory::createSpell(SpellType type) {
    switch (type) {
        case SpellType::FIRE:
            return std::make_unique<FireSpell>();
        case SpellType::HEALING:
            return std::make_unique<HealingSpell>();
        case SpellType::SHIELD:
            return std::make_unique<ShieldSpell>();
        case SpellType::DARK_ATTACK:
            return std::make_unique<DarkAttackSpell>();
        case SpellType::BUBBLE_WAVE:
            return std::make_unique<BubbleWaveSpell>();
        case SpellType::MERGE_MAGIC:
            return std::make_unique<MergeMagicSpell>();
        case SpellType::QUICK_STRIKE:
            return std::make_unique<QuickStrikeSpell>();
    }
    return nullptr;
}

std::unique_ptr<Spell> SpellFactory::createSpellByName(const std::string& name) {
    if (name == "Fire Spell" || name == "Fire") {
        return std::make_unique<FireSpell>();
    } else if (name == "Healing Spell" || name == "Healing" || name == "Heal") {
        return std::make_unique<HealingSpell>();
    } else if (name == "Shield Spell" || name == "Shield") {
        return std::make_unique<ShieldSpell>();
    } else if (name == "Dark Attack" || name == "Dark") {
        return std::make_unique<DarkAttackSpell>();
    } else if (name == "Bubble Wave" || name == "Bubble") {
        return std::make_unique<BubbleWaveSpell>();
    } else if (name == "Merge Magic" || name == "Merge") {
        return std::make_unique<MergeMagicSpell>();
    } else if (name == "Quick Strike" || name == "Quick") {
        return std::make_unique<QuickStrikeSpell>();
    }
    return nullptr;
}
