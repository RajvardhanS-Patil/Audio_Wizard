#pragma once

#include "BaseSpell.hpp"

class FireSpell : public BaseSpell {
public:
    FireSpell();

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};
