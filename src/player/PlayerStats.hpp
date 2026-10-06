#pragma once

class PlayerStats {
public:
    PlayerStats();

    int getHealth() const;
    int getMaxHealth() const;
    void takeDamage(int amount);
    void restoreHealth(int amount);

    int getMana() const;
    int getMaxMana() const;
    void consumeMana(int amount);
    void restoreMana(int amount);

    int getLevel() const;
    int getXP() const;
    void addExperience(int amount);

    int getAttack() const;
    int getDefense() const;
    float getSpellPowerMultiplier() const;

    float getDSAMastery() const; // 0.0f to 100.0f
    void setDSAMastery(float mastery);
    void addDSAMastery(float amount);

    float getSpeed() const;

    // Passive regeneration and stat ticks
    void update(float deltaTime);

private:
    int m_health;
    int m_maxHealth;
    
    int m_mana;
    int m_maxMana;

    int m_level;
    int m_xp;

    int m_attack;
    int m_defense;
    float m_speed;

    float m_manaRegenAccumulator{0.0f};
    float m_dsaMastery;
};
