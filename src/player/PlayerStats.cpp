#include "PlayerStats.hpp"
#include <algorithm>

PlayerStats::PlayerStats() 
    : m_health(100), m_maxHealth(100)
    , m_mana(100), m_maxMana(100)
    , m_level(1), m_xp(0)
    , m_attack(10), m_defense(5), m_speed(250.0f)
    , m_dsaMastery(0.0f)
{
}

int PlayerStats::getHealth() const { return m_health; }
int PlayerStats::getMaxHealth() const { return m_maxHealth; }

void PlayerStats::takeDamage(int amount) {
    m_health -= amount;
    if (m_health < 0) m_health = 0;
}

void PlayerStats::restoreHealth(int amount) {
    m_health += amount;
    if (m_health > m_maxHealth) m_health = m_maxHealth;
}

int PlayerStats::getMana() const { return m_mana; }
int PlayerStats::getMaxMana() const { return m_maxMana; }

void PlayerStats::consumeMana(int amount) {
    m_mana -= amount;
    if (m_mana < 0) m_mana = 0;
}

void PlayerStats::restoreMana(int amount) {
    m_mana += amount;
    if (m_mana > m_maxMana) m_mana = m_maxMana;
}

int PlayerStats::getLevel() const { return m_level; }
int PlayerStats::getXP() const { return m_xp; }

void PlayerStats::addExperience(int amount) {
    m_xp += amount;
    // Level up logic
    while (m_xp >= m_level * 100) {
        m_xp -= m_level * 100;
        m_level++;
        m_maxHealth += 15;
        m_health = m_maxHealth;
        m_maxMana += 15;
        m_mana = m_maxMana;
        m_attack += 3;
        m_defense += 2;
    }
}

int PlayerStats::getAttack() const { return m_attack; }
int PlayerStats::getDefense() const { return m_defense; }

float PlayerStats::getSpellPowerMultiplier() const {
    // 1.0 base + 0.1 per level above 1 + DSA mastery bonus
    return 1.0f + (m_level - 1) * 0.15f + (m_dsaMastery * 0.005f);
}

float PlayerStats::getDSAMastery() const { return m_dsaMastery; }

void PlayerStats::setDSAMastery(float mastery) {
    m_dsaMastery = std::clamp(mastery, 0.0f, 100.0f);
}

void PlayerStats::addDSAMastery(float amount) {
    setDSAMastery(m_dsaMastery + amount);
}

float PlayerStats::getSpeed() const { return m_speed; }

void PlayerStats::update(float deltaTime) {
    // Passive mana regeneration: ~4 mana per second
    m_manaRegenAccumulator += deltaTime * 4.0f;
    if (m_manaRegenAccumulator >= 1.0f) {
        int add = static_cast<int>(m_manaRegenAccumulator);
        m_manaRegenAccumulator -= add;
        restoreMana(add);
    }
}
