#include "DamageSystem.hpp"
#include <algorithm>
#include <cmath>

int DamageSystem::calculateDamage(int rawDamage, float spellPowerMultiplier, float comboMultiplier, int targetDefense) {
    float scaled = rawDamage * spellPowerMultiplier * comboMultiplier;
    // Standard RPG defense mitigation: Effective Damage = Scaled * (100 / (100 + Defense))
    float mitigationFactor = 100.0f / (100.0f + std::max(0, targetDefense));
    int finalDamage = static_cast<int>(std::round(scaled * mitigationFactor));
    return std::max(1, finalDamage); // Minimum 1 damage
}

int DamageSystem::calculateHealing(int baseHealing, float spellPowerMultiplier) {
    return static_cast<int>(std::round(baseHealing * spellPowerMultiplier));
}
