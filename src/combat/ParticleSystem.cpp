#include "ParticleSystem.hpp"
#include <cmath>
#include <cstdlib>

ParticleSystem::ParticleSystem(size_t maxParticles)
    : m_particles(maxParticles), m_nextIndex(0) {
}

void ParticleSystem::clear() {
    for (auto& p : m_particles) {
        p.active = false;
    }
    m_nextIndex = 0;
}

void ParticleSystem::update(float deltaTime) {
    for (auto& p : m_particles) {
        if (!p.active) continue;

        p.lifetime -= deltaTime;
        if (p.lifetime <= 0.0f) {
            p.active = false;
            continue;
        }

        p.position.x += p.velocity.x * deltaTime;
        p.position.y += p.velocity.y * deltaTime;

        // Friction / drag
        p.velocity.x *= (1.0f - 1.2f * deltaTime);
        p.velocity.y *= (1.0f - 1.2f * deltaTime);
    }
}

void ParticleSystem::draw() const {
    for (const auto& p : m_particles) {
        if (!p.active) continue;

        float alphaRatio = (p.maxLifetime > 0.0f) ? (p.lifetime / p.maxLifetime) : 0.0f;
        if (alphaRatio < 0.0f) alphaRatio = 0.0f;
        if (alphaRatio > 1.0f) alphaRatio = 1.0f;

        Color renderColor = p.color;
        renderColor.a = static_cast<unsigned char>(p.color.a * alphaRatio);

        float currentSize = p.size * (0.4f + 0.6f * alphaRatio);
        DrawCircleV(p.position, currentSize, renderColor);
    }
}

void ParticleSystem::emit(Vector2 position, int count, Color color, float speed, float lifetime, float size) {
    for (int i = 0; i < count; ++i) {
        auto& p = m_particles[m_nextIndex];
        m_nextIndex = (m_nextIndex + 1) % m_particles.size();

        float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);
        float randSpeed = speed * (0.4f + 0.6f * (static_cast<float>(rand() % 100) / 100.0f));

        p.position = position;
        p.velocity = {std::cos(angle) * randSpeed, std::sin(angle) * randSpeed};
        p.color = color;
        p.size = size;
        p.lifetime = lifetime * (0.6f + 0.4f * (static_cast<float>(rand() % 100) / 100.0f));
        p.maxLifetime = p.lifetime;
        p.active = true;
    }
}

void ParticleSystem::emitBurst(Vector2 position, Color color, int count, float speed) {
    emit(position, count, color, speed, 0.6f, 4.0f);
}

void ParticleSystem::emitFireTrail(Vector2 position) {
    for (int i = 0; i < 3; ++i) {
        auto& p = m_particles[m_nextIndex];
        m_nextIndex = (m_nextIndex + 1) % m_particles.size();

        float offsetX = (rand() % 10 - 5) * 1.0f;
        float offsetY = (rand() % 10 - 5) * 1.0f;

        p.position = {position.x + offsetX, position.y + offsetY};
        p.velocity = {(rand() % 40 - 20) * 1.0f, -static_cast<float>(rand() % 40 + 20)}; // Float up
        p.color = (rand() % 2 == 0) ? Color{255, 120, 20, 220} : Color{255, 220, 50, 240};
        p.size = 3.5f;
        p.lifetime = 0.35f;
        p.maxLifetime = 0.35f;
        p.active = true;
    }
}

void ParticleSystem::emitHealingSparkles(Vector2 position) {
    for (int i = 0; i < 24; ++i) {
        auto& p = m_particles[m_nextIndex];
        m_nextIndex = (m_nextIndex + 1) % m_particles.size();

        float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);
        float dist = static_cast<float>(rand() % 35);
        p.position = {position.x + std::cos(angle) * dist, position.y + std::sin(angle) * dist};
        p.velocity = {std::cos(angle) * 20.0f, -static_cast<float>(rand() % 60 + 40)}; // Rise gracefully
        p.color = (rand() % 2 == 0) ? Color{80, 240, 120, 255} : Color{250, 240, 100, 255};
        p.size = 3.0f;
        p.lifetime = 0.7f;
        p.maxLifetime = 0.7f;
        p.active = true;
    }
}

void ParticleSystem::emitShieldSparks(Vector2 position) {
    for (int i = 0; i < 18; ++i) {
        auto& p = m_particles[m_nextIndex];
        m_nextIndex = (m_nextIndex + 1) % m_particles.size();

        float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);
        p.position = {position.x + std::cos(angle) * 25.0f, position.y + std::sin(angle) * 25.0f};
        p.velocity = {-std::sin(angle) * 70.0f, std::cos(angle) * 70.0f}; // Swirl tangential
        p.color = (rand() % 2 == 0) ? SKYBLUE : Color{200, 240, 255, 255};
        p.size = 3.5f;
        p.lifetime = 0.5f;
        p.maxLifetime = 0.5f;
        p.active = true;
    }
}

void ParticleSystem::emitDarkTendrils(Vector2 position) {
    for (int i = 0; i < 20; ++i) {
        auto& p = m_particles[m_nextIndex];
        m_nextIndex = (m_nextIndex + 1) % m_particles.size();

        float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);
        float speed = static_cast<float>(rand() % 90 + 30);
        p.position = position;
        p.velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        p.color = (rand() % 2 == 0) ? Color{140, 30, 200, 240} : Color{40, 10, 60, 255};
        p.size = 4.5f;
        p.lifetime = 0.55f;
        p.maxLifetime = 0.55f;
        p.active = true;
    }
}

void ParticleSystem::emitEnemyHit(Vector2 position) {
    emit(position, 15, Color{255, 80, 80, 255}, 140.0f, 0.4f, 3.5f);
    emit(position, 8, WHITE, 90.0f, 0.25f, 2.5f);
}

void ParticleSystem::emitEnemyDeath(Vector2 position) {
    emit(position, 35, Color{160, 40, 220, 255}, 160.0f, 0.9f, 5.0f);
    emit(position, 25, Color{40, 20, 60, 255}, 100.0f, 1.2f, 6.0f);
    emit(position, 15, SKYBLUE, 120.0f, 0.8f, 3.5f); // Cleansing light released
}

void ParticleSystem::emitCrystalCleanse(Vector2 position) {
    for (int i = 0; i < 60; ++i) {
        auto& p = m_particles[m_nextIndex];
        m_nextIndex = (m_nextIndex + 1) % m_particles.size();

        float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);
        float speed = static_cast<float>(rand() % 200 + 60);
        p.position = position;
        p.velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        p.color = (i % 3 == 0) ? SKYBLUE : ((i % 3 == 1) ? GOLD : WHITE);
        p.size = 4.0f;
        p.lifetime = 1.2f;
        p.maxLifetime = 1.2f;
        p.active = true;
    }
}
