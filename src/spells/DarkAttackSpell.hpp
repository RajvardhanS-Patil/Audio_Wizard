#pragma once

#include "BaseSpell.hpp"

class DarkAttackSpell : public BaseSpell {
public:
    DarkAttackSpell();

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};
