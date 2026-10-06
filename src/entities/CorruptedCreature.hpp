#pragma once

#include "Enemy.hpp"
#include <vector>
#include <string>

enum class CreatureState {
    IDLE,
    PATROL,
    DETECT,
    CHASE,
    ATTACK,
    TAKE_DAMAGE,
    DIE,
    DEFEATED
};

class CorruptedCreature : public Enemy {
public:
    CorruptedCreature(Vector2 startPos = {450.0f, 450.0f});

    void update(float deltaTime) override {}
    void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) override;
    void draw() const override;

    void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) override;

    bool isDead() const override { return m_state == CreatureState::DIE || m_state == CreatureState::DEFEATED; }
    bool isDefeated() const override { return m_state == CreatureState::DEFEATED; }
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    int getXPReward() const override { return m_xpReward; }
    Rectangle getBounds() const override;
    const std::string& getName() const override { return m_name; }
    bool isInCombat() const override;
    CreatureState getState() const { return m_state; }

    bool hasAwardedXP() const { return m_xpAwarded; }
    void markXPAwarded() { m_xpAwarded = true; }

private:
    void switchState(CreatureState newState);
    void updateIdle(float dt, Wizard& wizard, AudioManager& audioManager);
    void updatePatrol(float dt, Wizard& wizard, AudioManager& audioManager);
    void updateDetect(float dt, Wizard& wizard);
    void updateChase(float dt, Wizard& wizard, AudioManager& audioManager);
    void updateAttack(float dt, Wizard& wizard, AudioManager& audioManager, ParticleSystem& particles);
    void updateTakeDamage(float dt);
    void updateDie(float dt, ParticleSystem& particles);

    CreatureState m_state{CreatureState::IDLE};
    std::string m_name{"Corrupted Grove Beast"};

    int m_health{120};
    int m_maxHealth{120};
    int m_attackDamage{14};
    int m_defense{4};
    int m_xpReward{100};

    float m_patrolSpeed{90.0f};
    float m_chaseSpeed{160.0f};
    float m_detectionRadius{280.0f};
    float m_attackRange{48.0f};

    float m_attackCooldownTimer{0.0f};
    float m_attackCooldownDuration{1.4f};
    float m_stateTimer{0.0f};
    float m_hurtTimer{0.0f};
    float m_deathTimer{0.0f};

    std::vector<Vector2> m_patrolPoints;
    size_t m_currentPatrolIndex{0};

    int m_facingDirection{1};
    float m_radius{24.0f};
    bool m_xpAwarded{false};
};
