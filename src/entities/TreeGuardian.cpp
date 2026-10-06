#include "TreeGuardian.hpp"
#include "../player/Wizard.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../audio/AudioManager.hpp"
#include <raymath.h>

TreeGuardian::TreeGuardian(Vector2 position) {
    m_position = position;
}

void TreeGuardian::updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_health <= 0) return;

    m_pulseTime += deltaTime * 2.0f;
    m_attackTimer += deltaTime;

    Vector2 wPos = wizard.getPosition();
    float dist = Vector2Distance(m_position, wPos);

    if (dist < 800.0f) { // Aggro range
        if (m_attackTimer > 3.0f) {
            triggerRootAttack(wizard, particles, audioManager);
            m_attackTimer = 0.0f;
        }
    }
}

void TreeGuardian::triggerRootAttack(Wizard& wizard, ParticleSystem& particles, AudioManager& audioManager) {
    Vector2 targetPos = wizard.getPosition();
    audioManager.playSound("guardian_attack"); // Placeholder
    
    // Spawn root attack particle
    particles.emit(targetPos, 20, DARKGREEN, 50.0f, 1.0f, 6.0f);
    
    
    // Simple damage to wizard if standing still
    if (Vector2Distance(m_position, targetPos) < 800.0f) {
        wizard.getStats().takeDamage(15);
    }
}

void TreeGuardian::draw() const {
    if (m_health <= 0) return;

    // Draw Guardian body
    DrawCircleV(m_position, 60.0f, Fade(BROWN, 0.8f));
    DrawCircleLines(m_position.x, m_position.y, 60.0f, DARKGREEN);
    
    // Glowing eyes
    float eyePulse = 5.0f + 2.0f * sinf(m_pulseTime);
    DrawCircle(m_position.x - 20, m_position.y - 10, eyePulse, GOLD);
    DrawCircle(m_position.x + 20, m_position.y - 10, eyePulse, GOLD);
    
    // Health bar
    DrawRectangle(m_position.x - 50, m_position.y - 80, 100, 10, GRAY);
    DrawRectangle(m_position.x - 50, m_position.y - 80, (m_health / m_maxHealth) * 100, 10, RED);
    DrawText(m_name.c_str(), m_position.x - 40, m_position.y - 100, 16, WHITE);
}

void TreeGuardian::takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) {
    m_health -= damage;
    if (m_health <= 0) {
        audioManager.playSound("enemy_death");
        particles.emitBurst(m_position, GOLD, 30, 100.0f);
    }
}

Rectangle TreeGuardian::getBounds() const {
    return { m_position.x - 60.0f, m_position.y - 60.0f, 120.0f, 120.0f };
}
