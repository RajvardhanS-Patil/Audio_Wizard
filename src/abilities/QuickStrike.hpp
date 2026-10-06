#pragma once

#include "../dsa/DynamicArray.hpp"
#include <raylib.h>
#include <vector>
#include <memory>

class Wizard;
class Enemy;
class AudioManager;
class ParticleSystem;
class FloatingTextManager;
class PlayerStats;

class QuickStrike {
public:
    QuickStrike();

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

private:
    bool m_unlocked{false};
    int m_manaCost{35};
    float m_cooldownDuration{8.0f};
    float m_currentCooldown{0.0f};
    float m_activeTimer{0.0f};
    const float m_duration{1.5f};

    Vector2 m_pivotTargetPos{0.0f, 0.0f};
    bool m_pivotChosen{false};
    std::vector<Vector2> m_partitionStrikePoints;
};
