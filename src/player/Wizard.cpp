#include "Wizard.hpp"
#include <cmath>
#include "../combat/ParticleSystem.hpp"
#include "../audio/AudioManager.hpp"

Wizard::Wizard() {
    m_position = { 200.0f, 300.0f };
    m_position = { 200.0f, 300.0f };
}

void Wizard::update(float deltaTime, const InputManager* inputManager, Rectangle worldBounds) {
    if (!inputManager) return;

    Vector2 movement = { 0.0f, 0.0f };
    float speed = 200.0f * deltaTime;

    if (inputManager->isKeyDown(KEY_W)) movement.y -= speed;
    if (inputManager->isKeyDown(KEY_S)) movement.y += speed;
    if (inputManager->isKeyDown(KEY_A)) movement.x -= speed;
    if (inputManager->isKeyDown(KEY_D)) movement.x += speed;

    if (movement.x != 0.0f || movement.y != 0.0f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x = (movement.x / length) * speed;
        movement.y = (movement.y / length) * speed;
        
        if (movement.x > 0.0f) m_facingDirection = 1;
        if (movement.x < 0.0f) m_facingDirection = -1;
    }

    m_position.x += movement.x;
    m_position.y += movement.y;

    // Bounds check
    if (m_position.x - m_radius < worldBounds.x) m_position.x = worldBounds.x + m_radius;
    if (m_position.x + m_radius > worldBounds.width) m_position.x = worldBounds.width - m_radius;
    if (m_position.y - m_radius < worldBounds.y) m_position.y = worldBounds.y + m_radius;
    if (m_position.y + m_radius > worldBounds.height) m_position.y = worldBounds.height - m_radius;

    // Shield and hurt timers
    if (m_shieldTimer > 0.0f) m_shieldTimer -= deltaTime;
    if (m_hurtTimer > 0.0f) m_hurtTimer -= deltaTime;


}

void Wizard::draw() const {
    float time = GetTime();
    
    // Animation offsets
    float floatOffset = std::sin(time * 3.0f) * 4.0f;
    float crystalFloat = std::sin(time * 5.0f) * 3.0f;
    
    // Position with float offset
    Vector2 p = { m_position.x, m_position.y + floatOffset };
    
    Color mainRobe = isHurt() ? Color{255, 100, 100, 255} : Color{ 30, 40, 80, 255 };
    Color hatColor = isHurt() ? Color{200, 50, 50, 255} : Color{ 20, 25, 55, 255 };
    Color crystalColor = hasShield() ? Color{50, 200, 255, 255} : Color{ 100, 255, 200, 255 };

    // Draw magic aura (subtle back glow)
    DrawCircleGradient(Vector2{p.x, p.y}, 45.0f, Color{40, 100, 200, 40}, BLANK);

    // Shadow
    DrawEllipse(m_position.x, m_position.y + 25, 18, 6, Color{0, 0, 0, 100});

    // Wizard Robe (Triangle with rounded bottom)
    DrawTriangle(
        {p.x, p.y - 15.0f},
        {p.x - 15.0f, p.y + 20.0f},
        {p.x + 15.0f, p.y + 20.0f},
        mainRobe
    );

    // Hat
    DrawTriangle(
        {p.x, p.y - 35.0f},
        {p.x - 12.0f, p.y - 10.0f},
        {p.x + 12.0f, p.y - 10.0f},
        hatColor
    );
    DrawRectangleRounded({p.x - 18.0f, p.y - 12.0f, 36.0f, 6.0f}, 0.5f, 4, GOLD);

    // Face/Eyes
    DrawCircle(p.x + (4 * m_facingDirection), p.y - 5, 2, SKYBLUE);

    // Staff
    float staffX = p.x + (18.0f * m_facingDirection);
    DrawRectangle(staffX - 2, p.y - 20, 4, 45, Color{ 100, 60, 30, 255 });
    
    // Staff Crystal
    float cx = staffX;
    float cy = p.y - 25.0f + crystalFloat;
    DrawCircleGradient(Vector2{cx, cy}, 12.0f, Color{255, 255, 255, 150}, BLANK);
    DrawPoly({cx, cy}, 6, 6.0f, time * 30.0f, crystalColor);
    DrawPolyLinesEx({cx, cy}, 6, 6.0f, time * 30.0f, 1.0f, WHITE);

    // Shield effect
    if (hasShield()) {
        DrawCircleLines(m_position.x, m_position.y, 35.0f + std::sin(time * 10.0f) * 2.0f, Color{ 100, 200, 255, 150 });
        DrawCircleGradient(Vector2{m_position.x, m_position.y}, 35.0f, Color{100, 200, 255, 50}, BLANK);
    }
}

Rectangle Wizard::getBounds() const {
    return { m_position.x - m_radius, m_position.y - m_radius, m_radius * 2, m_radius * 2 };
}

void Wizard::activateShield(float amount, float duration) {
    m_shieldHealth = amount;
    m_shieldTimer = duration;
}

float Wizard::absorbDamageWithShield(float damage) {
    if (!hasShield()) return damage;
    
    if (m_shieldHealth >= damage) {
        m_shieldHealth -= damage;
        return 0.0f;
    } else {
        float remainingDamage = damage - m_shieldHealth;
        m_shieldHealth = 0.0f;
        m_shieldTimer = 0.0f;
        return remainingDamage;
    }
}

void Wizard::triggerHurt() {
    m_hurtTimer = 0.3f;
}

void Wizard::castChainLink(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager) {
    if (m_chainLink.canActivate(m_stats)) {
        m_chainLink.activate(*this, enemies, audioManager, particles);
    }
}

void Wizard::castQueueSurge(CombatSystem& combatSystem, AudioManager& audioManager) {
    if (m_queueSurge.canActivate(m_stats)) {
        m_queueSurge.activate(*this, combatSystem, audioManager);
    }
}

void Wizard::castBubbleWave(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager) {
    if (m_bubbleWave.canActivate(m_stats)) {
        m_bubbleWave.activate(*this, enemies, audioManager, particles);
    }
}

void Wizard::castMergeMagic(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager) {
    if (m_mergeMagic.canActivate(m_stats)) {
        m_mergeMagic.activate(*this, enemies, audioManager, particles);
    }
}

void Wizard::castQuickStrike(std::vector<std::unique_ptr<Enemy>>& enemies, ParticleSystem& particles, AudioManager& audioManager) {
    if (m_quickStrike.canActivate(m_stats)) {
        m_quickStrike.activate(*this, enemies, audioManager, particles);
    }
}
