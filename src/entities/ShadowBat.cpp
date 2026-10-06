#include "ShadowBat.hpp"
#include "../player/Wizard.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include <raymath.h>
#include <cmath>
#include <algorithm>

ShadowBat::ShadowBat(Vector2 startPos) {
    m_position = startPos;
}

bool ShadowBat::isInCombat() const {
    return m_state == BatState::DETECT ||
           m_state == BatState::CHASE ||
           m_state == BatState::ATTACK ||
           m_state == BatState::TAKE_DAMAGE;
}

Rectangle ShadowBat::getBounds() const {
    return {
        m_position.x - m_radius,
        m_position.y - m_radius,
        m_radius * 2,
        m_radius * 2
    };
}

void ShadowBat::switchState(BatState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

void ShadowBat::takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) {
    if (isDead()) return;

    int actual = std::max(1, damage - m_defense);
    m_health -= actual;

    particles.emitEnemyHit(m_position);
    audioManager.playSound("enemy_hit");
    m_hurtTimer = 0.25f;

    if (m_health <= 0) {
        m_health = 0;
        switchState(BatState::DIE);
        audioManager.playSound("enemy_death");
        particles.emitEnemyDeath(m_position);
    } else {
        switchState(BatState::TAKE_DAMAGE);
    }
}

void ShadowBat::updateAI(float deltaTime, Wizard& wizard, const World& /*world*/, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_state == BatState::DEFEATED) return;

    if (m_attackCooldownTimer > 0.0f) {
        m_attackCooldownTimer -= deltaTime;
    }
    if (m_hurtTimer > 0.0f) {
        m_hurtTimer -= deltaTime;
    }

    m_stateTimer += deltaTime;

    switch (m_state) {
        case BatState::IDLE: {
            float dist = Vector2Distance(m_position, wizard.getPosition());
            if (dist < m_detectionRadius) {
                switchState(BatState::DETECT);
                audioManager.playSound("bat_screech");
            }
            break;
        }
        case BatState::PATROL:
        case BatState::DETECT: {
            m_facingDirection = (wizard.getPosition().x >= m_position.x) ? 1 : -1;
            if (m_stateTimer > 0.3f) {
                switchState(BatState::CHASE);
            }
            break;
        }
        case BatState::CHASE: {
            Vector2 target = wizard.getPosition();
            Vector2 dir = Vector2Subtract(target, m_position);
            float dist = Vector2Length(dir);
            m_facingDirection = (dir.x >= 0.0f) ? 1 : -1;

            if (dist <= m_attackRange && m_attackCooldownTimer <= 0.0f) {
                switchState(BatState::ATTACK);
                return;
            }

            if (dist > 1.0f) {
                dir = Vector2Scale(dir, 1.0f / dist);
                m_position = Vector2Add(m_position, Vector2Scale(dir, m_speed * deltaTime));
            }
            break;
        }
        case BatState::ATTACK: {
            if (m_stateTimer >= 0.15f && m_attackCooldownTimer <= 0.0f) {
                audioManager.playSound("bat_screech");
                particles.emitEnemyHit(wizard.getPosition());

                float dmg = static_cast<float>(m_attackDamage);
                float rem = wizard.absorbDamageWithShield(dmg);
                if (rem > 0.0f) {
                    int finalDmg = std::max(1, static_cast<int>(rem) - wizard.getStats().getDefense() / 2);
                    wizard.getStats().takeDamage(finalDmg);
                    wizard.triggerHurt();
                }
                m_attackCooldownTimer = 1.2f;
            }
            if (m_stateTimer >= 0.4f) {
                switchState(BatState::CHASE);
            }
            break;
        }
        case BatState::TAKE_DAMAGE: {
            if (m_stateTimer >= 0.25f) {
                switchState(BatState::CHASE);
            }
            break;
        }
        case BatState::DIE: {
            m_deathTimer += deltaTime;
            if (m_deathTimer >= 0.6f) {
                switchState(BatState::DEFEATED);
            }
            break;
        }
        case BatState::DEFEATED:
            break;
    }
}

void ShadowBat::draw() const {
    if (m_state == BatState::DEFEATED) return;

    float time = static_cast<float>(GetTime());
    float wingFlap = std::sin(time * 18.0f) * 12.0f;

    Color bodyColor = (m_hurtTimer > 0.0f) ? Color{255, 90, 90, 255} : Color{40, 20, 60, 255};
    Color wingColor = (m_hurtTimer > 0.0f) ? Color{240, 140, 140, 255} : Color{80, 40, 110, 255};

    // Body
    DrawEllipse(static_cast<int>(m_position.x), static_cast<int>(m_position.y), 10.0f, 13.0f, bodyColor);

    // Left Wing
    DrawTriangle(
        {m_position.x - 3.0f, m_position.y},
        {m_position.x - 22.0f, m_position.y - 10.0f + wingFlap},
        {m_position.x - 12.0f, m_position.y + 8.0f},
        wingColor
    );

    // Right Wing
    DrawTriangle(
        {m_position.x + 3.0f, m_position.y},
        {m_position.x + 12.0f, m_position.y + 8.0f},
        {m_position.x + 22.0f, m_position.y - 10.0f + wingFlap},
        wingColor
    );

    // Eyes
    float eyeX = m_position.x + m_facingDirection * 4.0f;
    DrawCircle(static_cast<int>(eyeX), static_cast<int>(m_position.y - 4.0f), 2.5f, RED);

    // Overhead Health Bar
    if (m_health < m_maxHealth || isInCombat()) {
        int barW = 40;
        int barH = 5;
        int barX = static_cast<int>(m_position.x - barW / 2);
        int barY = static_cast<int>(m_position.y - 22);

        DrawRectangle(barX - 1, barY - 1, barW + 2, barH + 2, Color{10, 10, 15, 200});
        float hpPct = static_cast<float>(m_health) / m_maxHealth;
        DrawRectangle(barX, barY, static_cast<int>(barW * hpPct), barH, Color{220, 50, 60, 255});
    }
}
