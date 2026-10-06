#pragma once

#include "../dsa/LinkedList.hpp"
#include <raylib.h>
#include <vector>
#include <memory>
#include <string>

class Wizard;
class Enemy;
class AudioManager;
class ParticleSystem;
class FloatingTextManager;
class PlayerStats;

class ChainLink {
public:
    ChainLink();

    void unlock() { m_unlocked = true; }
    bool isUnlocked() const { return m_unlocked; }

    bool canActivate(const PlayerStats& stats) const;
    void activate(Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                  AudioManager& audioManager, ParticleSystem& particles);

    void update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                AudioManager& audioManager, ParticleSystem& particles, FloatingTextManager& floatingText);
    void draw() const;

    float getCooldown() const { return m_cooldownDuration; }
    float getCurrentCooldown() const { return m_currentCooldown; }
    int getManaCost() const { return m_manaCost; }
    bool isActive() const { return m_activeTimer > 0.0f; }

    const LinkedList<Vector2>& getChainNodes() const { return m_chainNodes; }

private:
    bool m_unlocked{false};
    int m_manaCost{20};
    float m_cooldownDuration{4.0f};
    float m_currentCooldown{0.0f};
    float m_activeTimer{0.0f};
    const float m_duration{2.2f};
    float m_tickTimer{0.0f};

    // Linked List storing node coordinates along the energy chain
    LinkedList<Vector2> m_chainNodes;
};
