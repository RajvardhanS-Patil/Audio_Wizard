#include "BubbleWave.hpp"
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../combat/FloatingText.hpp"
#include <raymath.h>

BubbleWave::BubbleWave() = default;

bool BubbleWave::canActivate(const PlayerStats& stats) const {
    return m_unlocked && m_currentCooldown <= 0.0f && stats.getMana() >= m_manaCost;
}

void BubbleWave::activate(Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                        AudioManager& audioManager, ParticleSystem& particles) {
    if (!canActivate(wizard.getStats())) return;

    wizard.getStats().consumeMana(m_manaCost);
    m_currentCooldown = m_cooldownDuration;
    m_activeTimer = m_duration;
    m_waveRadius = 10.0f;
    m_waveOrigin = wizard.getPosition();

    // Populate dynamic array with nearby enemy power ratings for bubble comparison
    m_bubblePassValues.clear();
    for (const auto& enemy : enemies) {
        if (enemy && !enemy->isDead()) {
            m_bubblePassValues.push_back(static_cast<int>(enemy->getHealth()));
        }
    }

    // Play bubble wave compare audio notes
    audioManager.playNote("C4", 0.4f);
    audioManager.playNote("E4", 0.4f);
    audioManager.playNote("G4", 0.5f);

    // Create ripple particles
    particles.emitBurst(m_waveOrigin, SKYBLUE, 24, 120.0f);
}

void BubbleWave::update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                        AudioManager& audioManager, ParticleSystem& particles, FloatingTextManager& floatingText) {
    if (m_currentCooldown > 0.0f) {
        m_currentCooldown -= deltaTime;
        if (m_currentCooldown < 0.0f) m_currentCooldown = 0.0f;
    }

    if (m_activeTimer > 0.0f) {
        m_activeTimer -= deltaTime;
        m_waveRadius += 180.0f * deltaTime;

        // Perform Bubble Sort pass logic on nearby enemies
        for (size_t i = 0; i < enemies.size(); ++i) {
            auto& enemy = enemies[i];
            if (!enemy || enemy->isDead()) continue;

            float dist = Vector2Distance(m_waveOrigin, enemy->getPosition());
            if (fabsf(dist - m_waveRadius) < 35.0f) {
                // Apply bubble swap impact damage
                float damage = 35.0f;
                enemy->takeDamage(static_cast<int>(damage), audioManager, particles);
                floatingText.add(enemy->getPosition(), TextFormat("BUBBLE SWAP! -%.0f", damage), YELLOW);

                particles.emitBurst(enemy->getPosition(), GOLD, 15);
            }
        }
    }
}

void BubbleWave::draw() const {
    if (m_activeTimer > 0.0f) {
        DrawCircleLinesV(m_waveOrigin, m_waveRadius, SKYBLUE);
        DrawCircleLinesV(m_waveOrigin, m_waveRadius * 0.9f, GOLD);
    }
}
