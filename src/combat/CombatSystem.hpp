#pragma once

#include "DamageSystem.hpp"
#include "ParticleSystem.hpp"
#include "FloatingText.hpp"
#include <raylib.h>
#include <vector>
#include <memory>
#include <string>

class Wizard;
class Enemy;
class AudioManager;

struct SpellProjectile {
    Vector2 position{0.0f, 0.0f};
    Vector2 velocity{0.0f, 0.0f};
    float radius{7.0f};
    int damage{25};
    Color color{ORANGE};
    std::string type{"fire"};
    float lifetime{3.0f};
    bool active{false};
};

class CombatSystem {
public:
    CombatSystem();

    void update(float deltaTime, Wizard& wizard, std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager);
    void draw() const;

    void spawnProjectile(Vector2 startPos, Vector2 dir, float speed, int damage, Color color, const std::string& type);

    void addFloatingText(Vector2 pos, const std::string& text, Color color, int fontSize = 16);

    void registerSpellSuccess();
    void registerSpellMiss();
    void resetCombo();

    int getCombo() const { return m_comboCount; }
    float getComboMultiplier() const;
    float getComboTimer() const { return m_comboTimer; }

    ParticleSystem& getParticles() { return m_particles; }
    const ParticleSystem& getParticles() const { return m_particles; }

    FloatingTextManager& getFloatingText() { return m_floatingText; }

    void setActiveEnemies(std::vector<std::unique_ptr<Enemy>>* enemies) { m_activeEnemies = enemies; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return *m_activeEnemies; }

private:
    std::vector<std::unique_ptr<Enemy>>* m_activeEnemies{nullptr};
    std::vector<SpellProjectile> m_projectiles;
    ParticleSystem m_particles;
    FloatingTextManager m_floatingText;

    int m_comboCount{0};
    float m_comboTimer{0.0f};
    const float m_comboWindow{4.5f};
};
