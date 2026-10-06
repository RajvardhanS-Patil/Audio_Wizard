#pragma once

#include "Enemy.hpp"
#include <string>
#include <vector>

enum class GuardianState {
    IDLE,
    CHASE,
    ATTACK,
    TAKE_DAMAGE,
    DIE,
    DEFEATED
};

class ChainGuardian : public Enemy {
public:
    ChainGuardian(Vector2 startPos = {1200.0f, 700.0f});

    void update(float deltaTime) override {}
    void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) override;
    void draw() const override;

    void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) override;

    bool isDead() const override { return m_state == GuardianState::DIE || m_state == GuardianState::DEFEATED; }
    bool isDefeated() const override { return m_state == GuardianState::DEFEATED; }
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    int getXPReward() const override { return m_xpReward; }
    Rectangle getBounds() const override;
    const std::string& getName() const override { return m_name; }
    bool isInCombat() const override { return true; }

    bool isShielded() const { return m_shielded; }
    void breakChains(AudioManager& audioManager, ParticleSystem& particles);

    bool hasAwardedXP() const { return m_xpAwarded; }
    void markXPAwarded() { m_xpAwarded = true; }

private:
    void switchState(GuardianState newState);

    GuardianState m_state{GuardianState::IDLE};
    std::string m_name{"The Chain Guardian"};

    int m_health{240};
    int m_maxHealth{240};
    int m_attackDamage{22};
    int m_defense{6};
    int m_xpReward{250};

    bool m_shielded{true};
    float m_speed{85.0f};
    float m_attackRange{62.0f};

    float m_attackCooldownTimer{0.0f};
    float m_stateTimer{0.0f};
    float m_hurtTimer{0.0f};
    float m_deathTimer{0.0f};

    int m_facingDirection{1};
    float m_radius{36.0f};
    bool m_xpAwarded{false};
};
