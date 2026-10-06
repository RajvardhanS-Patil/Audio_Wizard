#pragma once

class DamageSystem {
public:
    static int calculateDamage(int rawDamage, float spellPowerMultiplier, float comboMultiplier, int targetDefense);
    static int calculateHealing(int baseHealing, float spellPowerMultiplier);
};
