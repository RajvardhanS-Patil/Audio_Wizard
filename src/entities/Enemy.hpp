#pragma once

#include "Entity.hpp"
#include <string>

class AudioManager;
class Wizard;
class World;
class ParticleSystem;

class Enemy : public Entity {
public:
    virtual ~Enemy() = default;

    virtual void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) = 0;
    virtual void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) = 0;
    virtual bool isDead() const = 0;
    virtual bool isDefeated() const = 0;
    virtual int getHealth() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual int getXPReward() const = 0;
    virtual Rectangle getBounds() const = 0;
    virtual const std::string& getName() const = 0;
    virtual bool isInCombat() const = 0;
};
