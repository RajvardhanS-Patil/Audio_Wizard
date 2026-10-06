#pragma once

#include "Enemy.hpp"
#include <string>

enum class BatState {
    IDLE,
    PATROL,
    DETECT,
    CHASE,
    ATTACK,
    TAKE_DAMAGE,
    DIE,
    DEFEATED
};

class ShadowBat : public Enemy {
public:
    ShadowBat(Vector2 startPos = {700.0f, 500.0f});

    void update(float deltaTime) override {}
    void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) override;
    void draw() const override;

    void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) override;

    bool isDead() const override { return m_state == BatState::DIE || m_state == BatState::DEFEATED; }
    bool isDefeated() const override { return m_state == BatState::DEFEATED; }
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    int getXPReward() const override { return m_xpReward; }
    Rectangle getBounds() const override;
    const std::string& getName() const override { return m_name; }
    bool isInCombat() const override;

    bool hasAwardedXP() const { return m_xpAwarded; }
    void markXPAwarded() { m_xpAwarded = true; }

private:
    void switchState(BatState newState);

    BatState m_state{BatState::IDLE};
    std::string m_name{"Shadow Bat"};

    int m_health{65};
    int m_maxHealth{65};
    int m_attackDamage{10};
    int m_defense{2};
    int m_xpReward{60};

    float m_speed{175.0f};
    float m_detectionRadius{260.0f};
    float m_attackRange{36.0f};

    float m_attackCooldownTimer{0.0f};
    float m_stateTimer{0.0f};
    float m_hurtTimer{0.0f};
    float m_deathTimer{0.0f};

    int m_facingDirection{1};
    float m_radius{16.0f};
    bool m_xpAwarded{false};
};
