#pragma once

#include "../entities/Entity.hpp"
#include "PlayerStats.hpp"
#include "../core/InputManager.hpp"
#include "../abilities/ChainLink.hpp"
#include "../abilities/QueueSurge.hpp"
#include "../abilities/BubbleWave.hpp"
#include "../abilities/MergeMagic.hpp"
#include "../abilities/QuickStrike.hpp"
#include <raymath.h>

class Wizard : public Entity {
public:
    Wizard();

    void update(float deltaTime) override {} // Base implementation
    void update(float deltaTime, const InputManager* inputManager, Rectangle worldBounds);
    
    void draw() const override;

    PlayerStats& getStats() { return m_stats; }
    const PlayerStats& getStats() const { return m_stats; }
    
    Rectangle getBounds() const;
    int getFacingDirection() const { return m_facingDirection; }

    // Shield barrier system
    void activateShield(float amount, float duration);
    bool hasShield() const { return m_shieldHealth > 0.0f && m_shieldTimer > 0.0f; }
    float getShieldHealth() const { return m_shieldHealth; }
    float getShieldTimer() const { return m_shieldTimer; }
    float absorbDamageWithShield(float damage);

    // Hit feedback
    void triggerHurt();
    bool isHurt() const { return m_hurtTimer > 0.0f; }

    // Phase 4 & 5 Abilities
    void castChainLink(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager);
    void castQueueSurge(CombatSystem& combatSystem, AudioManager& audioManager);
    void castBubbleWave(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager);
    void castMergeMagic(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager);
    void castQuickStrike(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager);

    bool isChainLinkReady() const { return m_chainLink.canActivate(m_stats); }
    bool isQueueSurgeReady() const { return m_queueSurge.canActivate(m_stats); }
    bool isBubbleWaveReady() const { return m_bubbleWave.canActivate(m_stats); }
    bool isMergeMagicReady() const { return m_mergeMagic.canActivate(m_stats); }
    bool isQuickStrikeReady() const { return m_quickStrike.canActivate(m_stats); }

    ChainLink& getChainLink() { return m_chainLink; }
    QueueSurge& getQueueSurge() { return m_queueSurge; }
    BubbleWave& getBubbleWave() { return m_bubbleWave; }
    MergeMagic& getMergeMagic() { return m_mergeMagic; }
    QuickStrike& getQuickStrike() { return m_quickStrike; }

private:
    PlayerStats m_stats;
    Vector2 m_velocity{ 0.0f, 0.0f };
    int m_facingDirection{ 1 }; // 1 for right, -1 for left
    
    // Shield state
    float m_shieldHealth{0.0f};
    float m_shieldTimer{0.0f};

    // Hurt flash timer
    float m_hurtTimer{0.0f};

    // Visual sizing
    float m_radius{ 20.0f };

    ChainLink m_chainLink;
    QueueSurge m_queueSurge;
    BubbleWave m_bubbleWave;
    MergeMagic m_mergeMagic;
    QuickStrike m_quickStrike;
};

