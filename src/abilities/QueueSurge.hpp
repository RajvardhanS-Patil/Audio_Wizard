#pragma once

#include "../dsa/Queue.hpp"
#include <raylib.h>
#include <vector>
#include <string>

class Wizard;
class CombatSystem;
class AudioManager;
class ParticleSystem;
class PlayerStats;

struct SurgePulse {
    Vector2 origin{0.0f, 0.0f};
    Vector2 direction{1.0f, 0.0f};
    float delay{0.0f};
    Color color{GOLD};
    int damage{25};
};

class QueueSurge {
public:
    QueueSurge();

    void unlock() { m_unlocked = true; }
    bool isUnlocked() const { return m_unlocked; }

    bool canActivate(const PlayerStats& stats) const;
    void activate(Wizard& wizard, CombatSystem& combatSystem, AudioManager& audioManager);

    void update(float deltaTime, CombatSystem& combatSystem, AudioManager& audioManager);

    float getCooldown() const { return m_cooldownDuration; }
    float getCurrentCooldown() const { return m_currentCooldown; }
    int getManaCost() const { return m_manaCost; }
    size_t getPendingCount() const { return m_pulseQueue.size(); }

private:
    bool m_unlocked{false};
    int m_manaCost{25};
    float m_cooldownDuration{3.5f};
    float m_currentCooldown{0.0f};

    // FIFO Queue managing sequentially dispatched arcane pulses
    Queue<SurgePulse> m_pulseQueue;
    float m_elapsed{0.0f};
};
