#pragma once

#include "Enemy.hpp"
#include <vector>
#include <string>

class TreeGuardian : public Enemy {
public:
    TreeGuardian(Vector2 position = {1200.0f, 600.0f});

    void update(float deltaTime) override {}
    void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) override;
    void draw() const override;

    void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) override;

    bool isDead() const override { return m_health <= 0; }
    bool isDefeated() const override { return m_health <= 0; }
    int getHealth() const override { return static_cast<int>(m_health); }
    int getMaxHealth() const override { return static_cast<int>(m_maxHealth); }
    int getXPReward() const override { return 500; }
    Rectangle getBounds() const override;
    const std::string& getName() const override { return m_name; }
    bool isInCombat() const override { return true; }

private:
    void triggerRootAttack(Wizard& wizard, ParticleSystem& particles, AudioManager& audioManager);

    std::string m_name{"Tree Guardian"};
    float m_health{500.0f};
    float m_maxHealth{500.0f};
    
    float m_attackTimer{0.0f};
    float m_pulseTime{0.0f};
    
    // We can add BST logic or node restoration phases here if needed for deeper boss logic
};
