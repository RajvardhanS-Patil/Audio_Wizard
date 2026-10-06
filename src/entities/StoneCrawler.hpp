#pragma once

#include "Enemy.hpp"
#include <string>

enum class CrawlerState {
    IDLE,
    PATROL,
    DETECT,
    CHASE,
    ATTACK,
    TAKE_DAMAGE,
    DIE,
    DEFEATED
};

class StoneCrawler : public Enemy {
public:
    StoneCrawler(Vector2 startPos = {800.0f, 650.0f});

    void update(float deltaTime) override {}
    void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) override;
    void draw() const override;

    void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) override;

    bool isDead() const override { return m_state == CrawlerState::DIE || m_state == CrawlerState::DEFEATED; }
    bool isDefeated() const override { return m_state == CrawlerState::DEFEATED; }
    int getHealth() const override { return m_health; }
    int getMaxHealth() const override { return m_maxHealth; }
    int getXPReward() const override { return m_xpReward; }
    Rectangle getBounds() const override;
    const std::string& getName() const override { return m_name; }
    bool isInCombat() const override;

    bool hasAwardedXP() const { return m_xpAwarded; }
    void markXPAwarded() { m_xpAwarded = true; }

private:
    void switchState(CrawlerState newState);

    CrawlerState m_state{CrawlerState::IDLE};
    std::string m_name{"Stone Crawler"};

    int m_health{130};
    int m_maxHealth{130};
    int m_attackDamage{18};
    int m_defense{8};
    int m_xpReward{80};

    float m_speed{75.0f};
    float m_detectionRadius{240.0f};
    float m_attackRange{44.0f};

    float m_attackCooldownTimer{0.0f};
    float m_stateTimer{0.0f};
    float m_hurtTimer{0.0f};
    float m_deathTimer{0.0f};

    int m_facingDirection{1};
    float m_radius{22.0f};
    bool m_xpAwarded{false};
};
