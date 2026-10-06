#include "CorruptedCreature.hpp"
#include "../player/Wizard.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include <raymath.h>
#include <cmath>
#include <algorithm>

CorruptedCreature::CorruptedCreature(Vector2 startPos) {
    m_position = startPos;

    // Patrol route through the corrupted grove
    m_patrolPoints.push_back({420.0f, 400.0f});
    m_patrolPoints.push_back({540.0f, 430.0f});
    m_patrolPoints.push_back({480.0f, 550.0f});
    m_patrolPoints.push_back({360.0f, 480.0f});
}

bool CorruptedCreature::isInCombat() const {
    return m_state == CreatureState::DETECT ||
           m_state == CreatureState::CHASE ||
           m_state == CreatureState::ATTACK ||
           m_state == CreatureState::TAKE_DAMAGE;
}

Rectangle CorruptedCreature::getBounds() const {
    return {
        m_position.x - m_radius,
        m_position.y - m_radius,
        m_radius * 2,
        m_radius * 2
    };
}

void CorruptedCreature::switchState(CreatureState newState) {
    m_state = newState;
    m_stateTimer = 0.0f;
}

void CorruptedCreature::takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) {
    if (isDead()) return;

    // Mitigation
    int actualDamage = std::max(1, damage - m_defense);
    m_health -= actualDamage;

    particles.emitEnemyHit(m_position);
    audioManager.playSound("enemy_hit");

    m_hurtTimer = 0.25f;

    if (m_health <= 0) {
        m_health = 0;
        switchState(CreatureState::DIE);
        audioManager.playSound("enemy_death");
        particles.emitEnemyDeath(m_position);
    } else {
        switchState(CreatureState::TAKE_DAMAGE);
    }
}

void CorruptedCreature::updateAI(float deltaTime, Wizard& wizard, const World& /*world*/, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_state == CreatureState::DEFEATED) return;

    if (m_attackCooldownTimer > 0.0f) {
        m_attackCooldownTimer -= deltaTime;
    }
    if (m_hurtTimer > 0.0f) {
        m_hurtTimer -= deltaTime;
    }

    m_stateTimer += deltaTime;

    switch (m_state) {
        case CreatureState::IDLE:
            updateIdle(deltaTime, wizard, audioManager);
            break;
        case CreatureState::PATROL:
            updatePatrol(deltaTime, wizard, audioManager);
            break;
        case CreatureState::DETECT:
            updateDetect(deltaTime, wizard);
            break;
        case CreatureState::CHASE:
            updateChase(deltaTime, wizard, audioManager);
            break;
        case CreatureState::ATTACK:
            updateAttack(deltaTime, wizard, audioManager, particles);
            break;
        case CreatureState::TAKE_DAMAGE:
            updateTakeDamage(deltaTime);
            break;
        case CreatureState::DIE:
            updateDie(deltaTime, particles);
            break;
        case CreatureState::DEFEATED:
            break;
    }
}

void CorruptedCreature::updateIdle(float /*dt*/, Wizard& wizard, AudioManager& audioManager) {
    float distToWizard = Vector2Distance(m_position, wizard.getPosition());
    if (distToWizard < m_detectionRadius) {
        switchState(CreatureState::DETECT);
        audioManager.playSound("enemy_detect");
        return;
    }

    if (m_stateTimer > 1.8f) {
        m_currentPatrolIndex = (m_currentPatrolIndex + 1) % m_patrolPoints.size();
        switchState(CreatureState::PATROL);
    }
}

void CorruptedCreature::updatePatrol(float dt, Wizard& wizard, AudioManager& audioManager) {
    float distToWizard = Vector2Distance(m_position, wizard.getPosition());
    if (distToWizard < m_detectionRadius) {
        switchState(CreatureState::DETECT);
        audioManager.playSound("enemy_detect");
        return;
    }

    Vector2 target = m_patrolPoints[m_currentPatrolIndex];
    Vector2 dir = Vector2Subtract(target, m_position);
    float dist = Vector2Length(dir);

    if (dist < 10.0f) {
        switchState(CreatureState::IDLE);
    } else {
        dir = Vector2Scale(dir, 1.0f / dist);
        m_facingDirection = (dir.x >= 0.0f) ? 1 : -1;
        m_position = Vector2Add(m_position, Vector2Scale(dir, m_patrolSpeed * dt));
    }
}

void CorruptedCreature::updateDetect(float /*dt*/, Wizard& wizard) {
    // Face the wizard during alert
    m_facingDirection = (wizard.getPosition().x >= m_position.x) ? 1 : -1;

    // After brief alert windup, begin chase
    if (m_stateTimer > 0.4f) {
        switchState(CreatureState::CHASE);
    }
}

void CorruptedCreature::updateChase(float dt, Wizard& wizard, AudioManager& /*audioManager*/) {
    Vector2 target = wizard.getPosition();
    Vector2 dir = Vector2Subtract(target, m_position);
    float dist = Vector2Length(dir);

    m_facingDirection = (dir.x >= 0.0f) ? 1 : -1;

    // If player escaped far beyond grove
    if (dist > m_detectionRadius * 1.6f) {
        switchState(CreatureState::PATROL);
        return;
    }

    // In attack range
    if (dist <= m_attackRange && m_attackCooldownTimer <= 0.0f) {
        switchState(CreatureState::ATTACK);
        return;
    }

    if (dist > 1.0f) {
        dir = Vector2Scale(dir, 1.0f / dist);
        m_position = Vector2Add(m_position, Vector2Scale(dir, m_chaseSpeed * dt));
    }
}

void CorruptedCreature::updateAttack(float /*dt*/, Wizard& wizard, AudioManager& audioManager, ParticleSystem& particles) {
    // Perform attack at ~0.2s into attack state
    if (m_stateTimer >= 0.2f && m_attackCooldownTimer <= 0.0f) {
        audioManager.playSound("enemy_attack");
        particles.emitEnemyHit(wizard.getPosition());

        float rawDamage = static_cast<float>(m_attackDamage);
        float remainingDamage = wizard.absorbDamageWithShield(rawDamage);

        if (remainingDamage > 0.0f) {
            // Apply defense mitigation
            int actualDamage = std::max(1, static_cast<int>(remainingDamage) - wizard.getStats().getDefense() / 2);
            wizard.getStats().takeDamage(actualDamage);
            wizard.triggerHurt();
        }

        m_attackCooldownTimer = m_attackCooldownDuration;
    }

    if (m_stateTimer >= 0.5f) {
        switchState(CreatureState::CHASE);
    }
}

void CorruptedCreature::updateTakeDamage(float /*dt*/) {
    if (m_stateTimer >= 0.35f) {
        switchState(CreatureState::CHASE);
    }
}

void CorruptedCreature::updateDie(float dt, ParticleSystem& particles) {
    m_deathTimer += dt;
    if (std::fmod(m_deathTimer, 0.15f) < dt) {
        particles.emitDarkTendrils(m_position);
    }

    if (m_deathTimer >= 0.85f) {
        switchState(CreatureState::DEFEATED);
    }
}

void CorruptedCreature::draw() const {
    if (m_state == CreatureState::DEFEATED) return;

    float time = static_cast<float>(GetTime());
    float breath = std::sin(time * 6.0f) * 1.5f;

    // Death shrink scale
    float scale = 1.0f;
    if (m_state == CreatureState::DIE) {
        scale = std::max(0.0f, 1.0f - (m_deathTimer / 0.85f));
    }

    float currentRadius = (m_radius + breath) * scale;
    if (currentRadius <= 0.5f) return;

    // Shadow
    DrawEllipse(static_cast<int>(m_position.x), static_cast<int>(m_position.y + m_radius * scale), 
                22.0f * scale, 8.0f * scale, Color{0, 0, 0, 110});

    // Body colors
    Color bodyColor = (m_hurtTimer > 0.0f) ? Color{255, 90, 90, 255} : Color{35, 15, 50, 255};
    Color spineColor = (m_hurtTimer > 0.0f) ? Color{255, 180, 180, 255} : Color{180, 40, 220, 255};

    // Quadruped beast body (main torso ellipse + head)
    DrawEllipse(static_cast<int>(m_position.x), static_cast<int>(m_position.y), 
                currentRadius * 1.15f, currentRadius * 0.85f, bodyColor);

    // Beast Head
    float headOffsetX = m_facingDirection * (currentRadius * 0.7f);
    Vector2 headPos = {m_position.x + headOffsetX, m_position.y - currentRadius * 0.3f};
    DrawCircleV(headPos, currentRadius * 0.65f, bodyColor);

    // Glowing Corrupted Spine Crystals
    for (int i = -2; i <= 2; ++i) {
        float spineX = m_position.x + i * (currentRadius * 0.35f);
        float spineHeight = (3 - std::abs(i)) * 6.0f * scale;
        Vector2 spineTop = {spineX, m_position.y - currentRadius * 0.85f - spineHeight};
        DrawTriangle(
            {spineX - 3.0f * scale, m_position.y - currentRadius * 0.7f},
            {spineX + 3.0f * scale, m_position.y - currentRadius * 0.7f},
            spineTop,
            spineColor
        );
    }

    // Glowing Horns on Head
    Vector2 hornBase = {headPos.x, headPos.y - currentRadius * 0.5f};
    DrawTriangle(
        {hornBase.x - 4.0f * scale, hornBase.y},
        {hornBase.x + 4.0f * scale, hornBase.y},
        {hornBase.x + m_facingDirection * 10.0f * scale, hornBase.y - 14.0f * scale},
        MAGENTA
    );

    // Glowing Crimson Eyes
    Vector2 eyePos = {headPos.x + m_facingDirection * 6.0f * scale, headPos.y - 2.0f * scale};
    DrawCircleV(eyePos, 4.0f * scale, RED);
    DrawCircleV(eyePos, 2.0f * scale, YELLOW); // Menacing pupil gleam

    // Detection alert visual (!)
    if (m_state == CreatureState::DETECT) {
        DrawRectangle(static_cast<int>(m_position.x - 3), static_cast<int>(m_position.y - m_radius - 28), 6, 14, RED);
        DrawCircle(static_cast<int>(m_position.x), static_cast<int>(m_position.y - m_radius - 8), 3.0f, RED);
    }

    // Overhead Health Bar (when engaged or damaged)
    if (m_health < m_maxHealth || isInCombat()) {
        int barW = 60;
        int barH = 6;
        int barX = static_cast<int>(m_position.x - barW / 2);
        int barY = static_cast<int>(m_position.y - m_radius - 16);

        DrawRectangle(barX - 1, barY - 1, barW + 2, barH + 2, Color{10, 10, 15, 220});
        float hpPct = static_cast<float>(m_health) / m_maxHealth;
        DrawRectangle(barX, barY, static_cast<int>(barW * hpPct), barH, Color{220, 40, 40, 255});
        DrawRectangleLines(barX - 1, barY - 1, barW + 2, barH + 2, DARKGRAY);
    }
}
