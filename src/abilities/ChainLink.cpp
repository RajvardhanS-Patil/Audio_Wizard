#include "ChainLink.hpp"
#include "../player/Wizard.hpp"
#include "../player/PlayerStats.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../combat/FloatingText.hpp"
#include <raymath.h>
#include <algorithm>

ChainLink::ChainLink() = default;

bool ChainLink::canActivate(const PlayerStats& stats) const {
    return m_unlocked && m_currentCooldown <= 0.0f && stats.getMana() >= m_manaCost;
}

void ChainLink::activate(Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                         AudioManager& audioManager, ParticleSystem& particles) {
    if (!canActivate(wizard.getStats())) return;

    wizard.getStats().consumeMana(m_manaCost);
    m_currentCooldown = m_cooldownDuration;
    m_activeTimer = m_duration;
    m_tickTimer = 0.0f;

    m_chainNodes.clear();

    // Node 0: Wizard (HEAD)
    m_chainNodes.insertBack(wizard.getPosition());

    // Search for closest enemies to link
    Vector2 pPos = wizard.getPosition();
    for (auto& enemy : enemies) {
        if (!enemy || enemy->isDead()) continue;

        float dist = Vector2Distance(pPos, enemy->getPosition());
        if (dist < 340.0f && m_chainNodes.size() < 4) {
            m_chainNodes.insertBack(enemy->getPosition());
            particles.emit(enemy->getPosition(), 10, SKYBLUE, 100.0f, 0.4f, 3.0f);
        }
    }

    audioManager.playSound("chain_link");
    particles.emit(wizard.getPosition(), 12, SKYBLUE, 90.0f, 0.4f, 3.5f);
}

void ChainLink::update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies,
                       AudioManager& audioManager, ParticleSystem& particles, FloatingTextManager& floatingText) {
    if (m_currentCooldown > 0.0f) {
        m_currentCooldown = std::max(0.0f, m_currentCooldown - deltaTime);
    }

    if (m_activeTimer <= 0.0f) {
        m_chainNodes.clear();
        return;
    }

    m_activeTimer -= deltaTime;
    m_tickTimer += deltaTime;

    // Update Head position to current wizard position
    if (!m_chainNodes.empty()) {
        m_chainNodes.getAt(0) = wizard.getPosition();
    }

    // Tick continuous chain damage every 0.35s
    if (m_tickTimer >= 0.35f) {
        m_tickTimer = 0.0f;

        for (auto& enemy : enemies) {
            if (!enemy || enemy->isDead()) continue;

            float dist = Vector2Distance(wizard.getPosition(), enemy->getPosition());
            if (dist < 340.0f) {
                int dmg = 12;
                enemy->takeDamage(dmg, audioManager, particles);
                floatingText.add(enemy->getPosition(), "-" + std::to_string(dmg) + " (Link)", SKYBLUE, 14);
            }
        }
    }
}

void ChainLink::draw() const {
    if (m_activeTimer <= 0.0f || m_chainNodes.size() < 2) return;

    std::vector<Vector2> nodes = m_chainNodes.toVector();
    float time = static_cast<float>(GetTime());

    for (size_t i = 0; i < nodes.size() - 1; ++i) {
        Vector2 start = nodes[i];
        Vector2 end = nodes[i + 1];

        // Draw lightning/chain beam
        DrawLineEx(start, end, 3.5f, SKYBLUE);
        DrawLineEx(start, end, 1.5f, WHITE);

        // Midpoint energy node
        Vector2 mid = Vector2Scale(Vector2Add(start, end), 0.5f);
        float pulse = std::sin(time * 12.0f + i) * 2.0f;
        DrawCircleV(mid, 4.0f + pulse, Color{150, 230, 255, 200});
    }
}
