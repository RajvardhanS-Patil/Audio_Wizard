#include "QuickStrike.hpp"
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../combat/FloatingText.hpp"
#include <raymath.h>

QuickStrike::QuickStrike() = default;

bool QuickStrike::canActivate(const PlayerStats& stats) const {
    return m_unlocked && m_currentCooldown <= 0.0f && stats.getMana() >= m_manaCost;
}

void QuickStrike::activate(Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                          AudioManager& audioManager, ParticleSystem& particles) {
    if (!canActivate(wizard.getStats())) return;

    wizard.getStats().consumeMana(m_manaCost);
    m_currentCooldown = m_cooldownDuration;
    m_activeTimer = m_duration;
    m_pivotChosen = false;

    // Find closest enemy as pivot target
    Enemy* closestEnemy = nullptr;
    float minDist = 99999.0f;
    for (const auto& enemy : enemies) {
        if (enemy && !enemy->isDead()) {
            float dist = Vector2Distance(wizard.getPosition(), enemy->getPosition());
            if (dist < minDist) {
                minDist = dist;
                closestEnemy = enemy.get();
            }
        }
    }

    if (closestEnemy) {
        m_pivotTargetPos = closestEnemy->getPosition();
        m_pivotChosen = true;
    } else {
        Vector2 dir = (wizard.getFacingDirection() > 0) ? Vector2{ 150.0f, 0.0f } : Vector2{ -150.0f, 0.0f };
        m_pivotTargetPos = Vector2Add(wizard.getPosition(), dir);
    }

    // Generate left and right partition strike points around pivot
    m_partitionStrikePoints.clear();
    m_partitionStrikePoints.push_back({ m_pivotTargetPos.x - 70.0f, m_pivotTargetPos.y - 40.0f });
    m_partitionStrikePoints.push_back({ m_pivotTargetPos.x - 50.0f, m_pivotTargetPos.y + 40.0f });
    m_partitionStrikePoints.push_back({ m_pivotTargetPos.x + 70.0f, m_pivotTargetPos.y - 40.0f });
    m_partitionStrikePoints.push_back({ m_pivotTargetPos.x + 50.0f, m_pivotTargetPos.y + 40.0f });

    // Play Quick Strike audio (C -> F -> G)
    audioManager.playNote("C4", 0.3f);
    audioManager.playNote("F4", 0.3f);
    audioManager.playNote("G4", 0.5f);

    particles.emitBurst(m_pivotTargetPos, PURPLE, 25);
}

void QuickStrike::update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                          AudioManager& audioManager, ParticleSystem& particles, FloatingTextManager& floatingText) {
    if (m_currentCooldown > 0.0f) {
        m_currentCooldown -= deltaTime;
        if (m_currentCooldown < 0.0f) m_currentCooldown = 0.0f;
    }

    if (m_activeTimer > 0.0f) {
        m_activeTimer -= deltaTime;

        // Partition strike damage logic
        if (m_activeTimer < 0.4f && m_activeTimer + deltaTime >= 0.4f) {
            for (auto& enemy : enemies) {
                if (enemy && !enemy->isDead()) {
                    float dist = Vector2Distance(m_pivotTargetPos, enemy->getPosition());
                    if (dist < 100.0f) {
                        float damage = 65.0f;
                        enemy->takeDamage(static_cast<int>(damage), audioManager, particles);
                        floatingText.add(enemy->getPosition(), TextFormat("QUICK PIVOT STRIKE! -%.0f", damage), PURPLE);
                        particles.emitBurst(enemy->getPosition(), PINK, 20);
                    }
                }
            }
        }
    }
}

void QuickStrike::draw() const {
    if (m_activeTimer > 0.0f) {
        // Draw pivot marker
        DrawCircleV(m_pivotTargetPos, 14.0f, PURPLE);
        DrawCircleLinesV(m_pivotTargetPos, 22.0f, PINK);

        // Draw partition strike lines
        for (const auto& strikePt : m_partitionStrikePoints) {
            DrawLineEx(strikePt, m_pivotTargetPos, 2.5f, MAGENTA);
            DrawCircleV(strikePt, 5.0f, ORANGE);
        }
    }
}
