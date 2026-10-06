#pragma once

#include "SpellCombination.hpp"
#include <string>
#include <raylib.h>

class Wizard;
class World;
class CombatSystem;
class AudioManager;
class PlayerStats;

class Spell {
public:
    virtual ~Spell() = default;

    virtual const std::string& getName() const = 0;
    virtual const std::string& getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual float getCooldown() const = 0;
    virtual float getCurrentCooldown() const = 0;
    virtual void updateCooldown(float dt) = 0;
    virtual void triggerCooldown() = 0;
    virtual void resetCooldown() = 0;
    virtual bool isReady() const = 0;
    virtual const SpellCombination& getCombination() const = 0;
    virtual Color getThemeColor() const = 0;

    virtual bool canCast(const PlayerStats& stats) const = 0;
    virtual bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) = 0;
};
