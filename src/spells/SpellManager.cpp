#include "SpellManager.hpp"
#include "SpellFactory.hpp"

SpellManager::SpellManager() {
    // Register Core Spells & Phase 5 Sorting Spells using SpellFactory
    registerSpell(SpellFactory::createSpell(SpellType::FIRE));
    registerSpell(SpellFactory::createSpell(SpellType::HEALING));
    registerSpell(SpellFactory::createSpell(SpellType::SHIELD));
    registerSpell(SpellFactory::createSpell(SpellType::DARK_ATTACK));
    registerSpell(SpellFactory::createSpell(SpellType::BUBBLE_WAVE));
    registerSpell(SpellFactory::createSpell(SpellType::MERGE_MAGIC));
    registerSpell(SpellFactory::createSpell(SpellType::QUICK_STRIKE));
}

void SpellManager::registerSpell(std::unique_ptr<Spell> spell) {
    if (spell) {
        m_spells.push_back(std::move(spell));
    }
}

void SpellManager::update(float deltaTime) {
    for (auto& spell : m_spells) {
        spell->updateCooldown(deltaTime);
    }
}

Spell* SpellManager::findMatchingSpell(const std::vector<NotePitch>& sequence) {
    for (auto& spell : m_spells) {
        if (spell->getCombination().matches(sequence)) {
            return spell.get();
        }
    }
    return nullptr;
}

Spell* SpellManager::getSpellByName(const std::string& name) {
    for (auto& spell : m_spells) {
        if (spell->getName() == name) {
            return spell.get();
        }
    }
    return nullptr;
}
