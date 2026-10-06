#pragma once

#include <raylib.h>
#include <vector>

struct Particle {
    Vector2 position{0.0f, 0.0f};
    Vector2 velocity{0.0f, 0.0f};
    Color color{WHITE};
    float size{3.0f};
    float lifetime{0.0f};
    float maxLifetime{1.0f};
    bool active{false};
};

class ParticleSystem {
public:
    ParticleSystem(size_t maxParticles = 600);

    void update(float deltaTime);
    void draw() const;

    void emit(Vector2 position, int count, Color color, float speed, float lifetime, float size);
    void emitBurst(Vector2 position, Color color, int count, float speed = 120.0f);
    void emitFireTrail(Vector2 position);
    void emitHealingSparkles(Vector2 position);
    void emitShieldSparks(Vector2 position);
    void emitDarkTendrils(Vector2 position);
    void emitEnemyHit(Vector2 position);
    void emitEnemyDeath(Vector2 position);
    void emitCrystalCleanse(Vector2 position);

    void clear();

private:
    std::vector<Particle> m_particles;
    size_t m_nextIndex{0};
};
