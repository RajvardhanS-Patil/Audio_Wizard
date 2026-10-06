#include "StoneCrawler.hpp"
#include "../player/Wizard.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include <raymath.h>
#include <algorithm>

StoneCrawler::StoneCrawler(Vector2 startPos) {
    m_position = startPos;
}

bool StoneCrawler::isInCombat() const {
    return m_state == CrawlerState::DETECT ||
           m_state == CrawlerState::CHASE ||
           m_state == CrawlerState::ATTACK ||
           m_state == CrawlerState::TAKE_DAMAGE;
}

Rectangle StoneCrawler::getBounds() const {
    return {
        m_position.x - m_radius,
        m_position.y - m_radius,
        m_radius * 2,
        m_radius * 2
    };
}

void StoneCrawler::switchState(CrawlerState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

void StoneCrawler::takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) {
    if (isDead()) return;

    int actual = std::max(1, damage - m_defense);
    m_health -= actual;

    particles.emitEnemyHit(m_position);
    audioManager.playSound("enemy_hit");
    m_hurtTimer = 0.25f;

    if (m_health <= 0) {
        m_health = 0;
        switchState(CrawlerState::DIE);
        audioManager.playSound("enemy_death");
        particles.emitEnemyDeath(m_position);
    } else {
        switchState(CrawlerState::TAKE_DAMAGE);
    }
}

void StoneCrawler::updateAI(float deltaTime, Wizard& wizard, const World& /*world*/, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_state == CrawlerState::DEFEATED) return;

    if (m_attackCooldownTimer > 0.0f) {
        m_attackCooldownTimer -= deltaTime;
    }
    if (m_hurtTimer > 0.0f) {
        m_hurtTimer -= deltaTime;
    }

    m_stateTimer += deltaTime;

    switch (m_state) {
        case CrawlerState::IDLE: {
            float dist = Vector2Distance(m_position, wizard.getPosition());
            if (dist < m_detectionRadius) {
                switchState(CrawlerState::DETECT);
                audioManager.playSound("crawler_slam");
            }
            break;
        }
        case CrawlerState::PATROL:
        case CrawlerState::DETECT: {
            m_facingDirection = (wizard.getPosition().x >= m_position.x) ? 1 : -1;
            if (m_stateTimer > 0.4f) {
                switchState(CrawlerState::CHASE);
            }
            break;
        }
        case CrawlerState::CHASE: {
            Vector2 target = wizard.getPosition();
            Vector2 dir = Vector2Subtract(target, m_position);
            float dist = Vector2Length(dir);
            m_facingDirection = (dir.x >= 0.0f) ? 1 : -1;

            if (dist <= m_attackRange && m_attackCooldownTimer <= 0.0f) {
                switchState(CrawlerState::ATTACK);
                return;
            }

            if (dist > 1.0f) {
                dir = Vector2Scale(dir, 1.0f / dist);
                m_position = Vector2Add(m_position, Vector2Scale(dir, m_speed * deltaTime));
            }
            break;
        }
        case CrawlerState::ATTACK: {
            if (m_stateTimer >= 0.2f && m_attackCooldownTimer <= 0.0f) {
                audioManager.playSound("crawler_slam");
                particles.emitEnemyHit(wizard.getPosition());

                float dmg = static_cast<float>(m_attackDamage);
                float rem = wizard.absorbDamageWithShield(dmg);
                if (rem > 0.0f) {
                    int finalDmg = std::max(1, static_cast<int>(rem) - wizard.getStats().getDefense() / 2);
                    wizard.getStats().takeDamage(finalDmg);
                    wizard.triggerHurt();
                }
                m_attackCooldownTimer = 1.6f;
            }
            if (m_stateTimer >= 0.6f) {
                switchState(CrawlerState::CHASE);
            }
            break;
        }
        case CrawlerState::TAKE_DAMAGE: {
            if (m_stateTimer >= 0.3f) {
                switchState(CrawlerState::CHASE);
            }
            break;
        }
        case CrawlerState::DIE: {
            m_deathTimer += deltaTime;
            if (m_deathTimer >= 0.7f) {
                switchState(CrawlerState::DEFEATED);
            }
            break;
        }
        case CrawlerState::DEFEATED:
            break;
    }
}

void StoneCrawler::draw() const {
    if (m_state == CrawlerState::DEFEATED) return;

    Color shellColor = (m_hurtTimer > 0.0f) ? Color{255, 120, 120, 255} : Color{90, 85, 95, 255};
    Color innerColor = (m_hurtTimer > 0.0f) ? Color{240, 100, 100, 255} : Color{50, 45, 55, 255};

    // Shadow
    DrawEllipse(static_cast<int>(m_position.x), static_cast<int>(m_position.y + m_radius * 0.8f), 
                m_radius * 1.2f, 6.0f, Color{0, 0, 0, 100});

    // Armored body
    DrawCircleV(m_position, m_radius, shellColor);
    DrawCircleV({m_position.x, m_position.y + 2.0f}, m_radius * 0.75f, innerColor);

    // Stone spikes on back
    for (int i = -1; i <= 1; ++i) {
        float sx = m_position.x + i * 8.0f;
        DrawTriangle(
            {sx - 4.0f, m_position.y - m_radius * 0.6f},
            {sx + 4.0f, m_position.y - m_radius * 0.6f},
            {sx, m_position.y - m_radius - 8.0f},
            GRAY
        );
    }

    // Glowing Amber Eyes
    float eyeX = m_position.x + m_facingDirection * 8.0f;
    DrawCircle(static_cast<int>(eyeX), static_cast<int>(m_position.y), 3.0f, ORANGE);
    DrawCircle(static_cast<int>(eyeX), static_cast<int>(m_position.y), 1.5f, YELLOW);

    // Overhead Health Bar
    if (m_health < m_maxHealth || isInCombat()) {
        int barW = 46;
        int barH = 5;
        int barX = static_cast<int>(m_position.x - barW / 2);
        int barY = static_cast<int>(m_position.y - 25);

        DrawRectangle(barX - 1, barY - 1, barW + 2, barH + 2, Color{10, 10, 15, 200});
        float hpPct = static_cast<float>(m_health) / m_maxHealth;
        DrawRectangle(barX, barY, static_cast<int>(barW * hpPct), barH, Color{220, 60, 40, 255});
    }
}
