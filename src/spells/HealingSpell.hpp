#pragma once

#include "BaseSpell.hpp"

class HealingSpell : public BaseSpell {
public:
    HealingSpell();

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};
