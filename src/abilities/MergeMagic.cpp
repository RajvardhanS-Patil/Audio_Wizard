#include "MergeMagic.hpp"
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../combat/FloatingText.hpp"
#include <raymath.h>

MergeMagic::MergeMagic() = default;

bool MergeMagic::canActivate(const PlayerStats& stats) const {
    return m_unlocked && m_currentCooldown <= 0.0f && stats.getMana() >= m_manaCost;
}

void MergeMagic::activate(Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                         AudioManager& audioManager, ParticleSystem& particles) {
    if (!canActivate(wizard.getStats())) return;

    wizard.getStats().consumeMana(m_manaCost);
    m_currentCooldown = m_cooldownDuration;
    m_activeTimer = m_duration;
    m_castPosition = wizard.getPosition();

    // Create split ray origins that converge into a merged point
    m_mergeRays.clear();
    for (int i = 0; i < 4; ++i) {
        float angle = (i / 4.0f) * 2.0f * PI;
        m_mergeRays.push_back({ m_castPosition.x + cosf(angle) * 120.0f, m_castPosition.y + sinf(angle) * 120.0f });
    }

    // Play Merge chord audio
    audioManager.playNote("D4", 0.4f);
    audioManager.playNote("F4", 0.4f);
    audioManager.playNote("A4", 0.6f);

    for (const auto& rayPos : m_mergeRays) {
        particles.emitBurst(rayPos, GREEN, 10);
    }
}

void MergeMagic::update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                         AudioManager& audioManager, ParticleSystem& particles, FloatingTextManager& floatingText) {
    if (m_currentCooldown > 0.0f) {
        m_currentCooldown -= deltaTime;
        if (m_currentCooldown < 0.0f) m_currentCooldown = 0.0f;
    }

    if (m_activeTimer > 0.0f) {
        m_activeTimer -= deltaTime;

        // Converge merge rays toward center cast position
        for (auto& rayPos : m_mergeRays) {
            Vector2 dir = Vector2Subtract(m_castPosition, rayPos);
            float len = Vector2Length(dir);
            if (len > 5.0f) {
                dir = Vector2Scale(Vector2Normalize(dir), 180.0f * deltaTime);
                rayPos = Vector2Add(rayPos, dir);
            }
        }

        // Deal merge sorting damage when rays converge
        if (m_activeTimer < 0.5f && m_activeTimer + deltaTime >= 0.5f) {
            for (auto& enemy : enemies) {
                if (enemy && !enemy->isDead()) {
                    float dist = Vector2Distance(m_castPosition, enemy->getPosition());
                    if (dist < 150.0f) {
                        float damage = 50.0f;
                        enemy->takeDamage(static_cast<int>(damage), audioManager, particles);
                        floatingText.add(enemy->getPosition(), TextFormat("MERGE CONVERGE! -%.0f", damage), LIME);
                        particles.emitBurst(enemy->getPosition(), GREEN, 20);
                    }
                }
            }
        }
    }
}

void MergeMagic::draw() const {
    if (m_activeTimer > 0.0f) {
        for (const auto& rayPos : m_mergeRays) {
            DrawLineEx(rayPos, m_castPosition, 3.0f, GREEN);
            DrawCircleV(rayPos, 6.0f, LIME);
        }
        DrawCircleV(m_castPosition, 10.0f, GOLD);
    }
}
