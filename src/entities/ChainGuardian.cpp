#include "ChainGuardian.hpp"
#include "../player/Wizard.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include <raymath.h>
#include <cmath>
#include <algorithm>

ChainGuardian::ChainGuardian(Vector2 startPos) {
    m_position = startPos;
}

Rectangle ChainGuardian::getBounds() const {
    return {
        m_position.x - m_radius,
        m_position.y - m_radius,
        m_radius * 2,
        m_radius * 2
    };
}

void ChainGuardian::switchState(GuardianState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

void ChainGuardian::breakChains(AudioManager& audioManager, ParticleSystem& particles) {
    if (!m_shielded) return;

    m_shielded = false;
    audioManager.playSound("guardian_vulnerable");
    particles.emitBurst(m_position, GOLD, 35, 140.0f);
    particles.emitBurst(m_position, SKYBLUE, 25, 120.0f);
}

void ChainGuardian::takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) {
    if (isDead()) return;

    // While shielded by the unbroken crystal chain, damage is blocked!
    if (m_shielded) {
        audioManager.playSound("chain_move");
        particles.emit(m_position, 12, SKYBLUE, 80.0f, 0.3f, 3.0f);
        return;
    }

    int actual = std::max(1, damage - m_defense);
    m_health -= actual;

    particles.emitEnemyHit(m_position);
    audioManager.playSound("enemy_hit");
    m_hurtTimer = 0.25f;

    if (m_health <= 0) {
        m_health = 0;
        switchState(GuardianState::DIE);
        audioManager.playSound("enemy_death");
        particles.emitEnemyDeath(m_position);
    } else {
        switchState(GuardianState::TAKE_DAMAGE);
    }
}

void ChainGuardian::updateAI(float deltaTime, Wizard& wizard, const World& /*world*/, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_state == GuardianState::DEFEATED) return;

    if (m_attackCooldownTimer > 0.0f) {
        m_attackCooldownTimer -= deltaTime;
    }
    if (m_hurtTimer > 0.0f) {
        m_hurtTimer -= deltaTime;
    }

    m_stateTimer += deltaTime;

    switch (m_state) {
        case GuardianState::IDLE: {
            float dist = Vector2Distance(m_position, wizard.getPosition());
            if (dist < 380.0f) {
                switchState(GuardianState::CHASE);
                audioManager.playSound("guardian_roar");
            }
            break;
        }
        case GuardianState::CHASE: {
            Vector2 target = wizard.getPosition();
            Vector2 dir = Vector2Subtract(target, m_position);
            float dist = Vector2Length(dir);
            m_facingDirection = (dir.x >= 0.0f) ? 1 : -1;

            if (dist <= m_attackRange && m_attackCooldownTimer <= 0.0f) {
                switchState(GuardianState::ATTACK);
                return;
            }

            if (dist > 1.0f) {
                dir = Vector2Scale(dir, 1.0f / dist);
                m_position = Vector2Add(m_position, Vector2Scale(dir, m_speed * deltaTime));
            }
            break;
        }
        case GuardianState::ATTACK: {
            if (m_stateTimer >= 0.25f && m_attackCooldownTimer <= 0.0f) {
                audioManager.playSound("chain_move");
                particles.emitEnemyHit(wizard.getPosition());

                float dmg = static_cast<float>(m_attackDamage);
                float rem = wizard.absorbDamageWithShield(dmg);
                if (rem > 0.0f) {
                    int finalDmg = std::max(1, static_cast<int>(rem) - wizard.getStats().getDefense() / 2);
                    wizard.getStats().takeDamage(finalDmg);
                    wizard.triggerHurt();
                }
                m_attackCooldownTimer = 1.8f;
            }
            if (m_stateTimer >= 0.65f) {
                switchState(GuardianState::CHASE);
            }
            break;
        }
        case GuardianState::TAKE_DAMAGE: {
            if (m_stateTimer >= 0.3f) {
                switchState(GuardianState::CHASE);
            }
            break;
        }
        case GuardianState::DIE: {
            m_deathTimer += deltaTime;
            if (m_deathTimer >= 1.2f) {
                switchState(GuardianState::DEFEATED);
            }
            break;
        }
        case GuardianState::DEFEATED:
            break;
    }
}

void ChainGuardian::draw() const {
    if (m_state == GuardianState::DEFEATED) return;

    float time = static_cast<float>(GetTime());
    Color bodyColor = (m_hurtTimer > 0.0f) ? Color{255, 130, 130, 255} : Color{55, 50, 75, 255};
    Color coreColor = m_shielded ? SKYBLUE : Color{255, 80, 80, 255};

    // Shadow
    DrawEllipse(static_cast<int>(m_position.x), static_cast<int>(m_position.y + m_radius), 
                m_radius * 1.3f, 10.0f, Color{0, 0, 0, 130});

    // Massive stone torso
    DrawCircleV(m_position, m_radius, bodyColor);
    DrawCircleLines(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_radius, DARKGRAY);

    // Glowing Heart Core (Protected or Exposed)
    DrawCircleV(m_position, 14.0f, coreColor);
    DrawCircleV(m_position, 7.0f, WHITE);

    // Stone Shoulders
    DrawCircle(static_cast<int>(m_position.x - m_radius * 0.8f), static_cast<int>(m_position.y - 10.0f), 12.0f, bodyColor);
    DrawCircle(static_cast<int>(m_position.x + m_radius * 0.8f), static_cast<int>(m_position.y - 10.0f), 12.0f, bodyColor);

    // Head with Glowing Horns
    Vector2 headPos = {m_position.x, m_position.y - m_radius * 0.85f};
    DrawCircleV(headPos, 16.0f, bodyColor);
    DrawCircle(static_cast<int>(headPos.x + m_facingDirection * 5.0f), static_cast<int>(headPos.y - 2.0f), 3.5f, coreColor);

    // Orbiting Chained Shield Crystals (if shielded)
    if (m_shielded) {
        float rotSpeed = time * 2.5f;
        for (int i = 0; i < 3; ++i) {
            float angle = rotSpeed + i * (2.0f * 3.14159f / 3.0f);
            Vector2 orbPos = {
                m_position.x + std::cos(angle) * (m_radius + 20.0f),
                m_position.y + std::sin(angle) * (m_radius + 20.0f)
            };
            DrawCircleV(orbPos, 7.0f, SKYBLUE);
            DrawCircleV(orbPos, 3.5f, WHITE);

            // Chain line to guardian core
            DrawLineEx(m_position, orbPos, 2.0f, Color{100, 200, 255, 140});
        }

        // Shield status text
        const char* sText = "[CHAIN SHIELD - SOLVE PUZZLE]";
        int tw = MeasureText(sText, 11);
        DrawText(sText, static_cast<int>(m_position.x - tw / 2), static_cast<int>(m_position.y - m_radius - 32), 11, SKYBLUE);
    }

    // Boss Health Bar
    int barW = 80;
    int barH = 7;
    int barX = static_cast<int>(m_position.x - barW / 2);
    int barY = static_cast<int>(m_position.y - m_radius - 18);
    DrawRectangle(barX - 1, barY - 1, barW + 2, barH + 2, Color{10, 10, 15, 230});
    float hpPct = static_cast<float>(m_health) / m_maxHealth;
    DrawRectangle(barX, barY, static_cast<int>(barW * hpPct), barH, m_shielded ? SKYBLUE : Color{220, 40, 40, 255});
}
