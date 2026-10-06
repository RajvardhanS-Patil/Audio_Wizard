#include "CombatSystem.hpp"
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include <raymath.h>
#include <algorithm>

CombatSystem::CombatSystem()
    : m_particles(600) {
    m_projectiles.reserve(64);
}

float CombatSystem::getComboMultiplier() const {
    // 1.0 base + 10% per combo stage, max 2.5x
    return std::min(2.5f, 1.0f + (m_comboCount * 0.10f));
}

void CombatSystem::registerSpellSuccess() {
    m_comboCount++;
    m_comboTimer = m_comboWindow;
}

void CombatSystem::registerSpellMiss() {
    resetCombo();
}

void CombatSystem::resetCombo() {
    m_comboCount = 0;
    m_comboTimer = 0.0f;
}

void CombatSystem::addFloatingText(Vector2 pos, const std::string& text, Color color, int fontSize) {
    m_floatingText.add(pos, text, color, fontSize);
}

void CombatSystem::spawnProjectile(Vector2 startPos, Vector2 dir, float speed, int damage, Color color, const std::string& type) {
    SpellProjectile proj;
    proj.position = startPos;
    float len = Vector2Length(dir);
    if (len > 0.001f) {
        dir = Vector2Scale(dir, 1.0f / len);
    } else {
        dir = {1.0f, 0.0f};
    }
    proj.velocity = Vector2Scale(dir, speed);
    proj.damage = damage;
    proj.color = color;
    proj.type = type;
    proj.radius = (type == "dark") ? 9.0f : 7.0f;
    proj.lifetime = 2.5f;
    proj.active = true;

    m_projectiles.push_back(proj);
}

void CombatSystem::update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager) {
    // Update Particles and Floating Texts
    m_particles.update(deltaTime);
    m_floatingText.update(deltaTime);

    // Update Combo Decay
    if (m_comboTimer > 0.0f) {
        m_comboTimer -= deltaTime;
        if (m_comboTimer <= 0.0f) {
            m_comboCount = 0;
        }
    }

    // Update Projectiles
    for (auto& proj : m_projectiles) {
        if (!proj.active) continue;

        proj.lifetime -= deltaTime;
        if (proj.lifetime <= 0.0f) {
            proj.active = false;
            continue;
        }

        proj.position.x += proj.velocity.x * deltaTime;
        proj.position.y += proj.velocity.y * deltaTime;

        // Particle trail
        if (proj.type == "fire") {
            m_particles.emitFireTrail(proj.position);
        } else if (proj.type == "dark") {
            m_particles.emitDarkTendrils(proj.position);
        }

        // Collision check against Enemies
        for (auto& enemy : enemies) {
            if (!enemy || enemy->isDead()) continue;

            if (CheckCollisionCircleRec(proj.position, proj.radius, enemy->getBounds())) {
                // Calculate damage with wizard spell power and combo multiplier
                float spellPower = wizard.getStats().getSpellPowerMultiplier();
                float comboMult = getComboMultiplier();
                int effectiveDamage = DamageSystem::calculateDamage(proj.damage, spellPower, comboMult, 4);

                enemy->takeDamage(effectiveDamage, audioManager, m_particles);

                // Combat feedback
                Color textColor = (proj.type == "dark") ? PURPLE : ORANGE;
                addFloatingText(enemy->getPosition(), "-" + std::to_string(effectiveDamage), textColor, 20);

                if (m_comboCount > 0) {
                    addFloatingText({wizard.getPosition().x, wizard.getPosition().y - 40.0f},
                                    "COMBO x" + std::to_string(m_comboCount + 1) + "!", GOLD, 16);
                }

                registerSpellSuccess();
                proj.active = false;
                break;
            }
        }
    }

    // Clean up inactive projectiles
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                       [](const SpellProjectile& p) { return !p.active; }),
        m_projectiles.end());
}

void CombatSystem::draw() const {
    // Draw active projectiles
    for (const auto& proj : m_projectiles) {
        if (!proj.active) continue;

        // Outer glow
        DrawCircleV(proj.position, proj.radius + 4.0f, Color{proj.color.r, proj.color.g, proj.color.b, 80});
        // Core orb
        DrawCircleV(proj.position, proj.radius, proj.color);
        // Inner hot core
        DrawCircleV(proj.position, proj.radius * 0.45f, WHITE);
    }

    // Draw particle bursts and effects
    m_particles.draw();

    // Draw floating damage numbers
    m_floatingText.draw();
}
