#pragma once

#include "BaseSpell.hpp"

class ShieldSpell : public BaseSpell {
public:
    ShieldSpell();

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};
