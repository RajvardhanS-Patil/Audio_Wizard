#pragma once

#include "Spell.hpp"
#include "../player/PlayerStats.hpp"
#include <algorithm>

class BaseSpell : public Spell {
public:
    BaseSpell(std::string name, std::string description, int manaCost, float cooldown,
              SpellCombination combination, Color themeColor)
        : m_name(std::move(name)),
          m_description(std::move(description)),
          m_manaCost(manaCost),
          m_cooldownDuration(cooldown),
          m_currentCooldown(0.0f),
          m_combination(std::move(combination)),
          m_themeColor(themeColor) {}

    const std::string& getName() const override { return m_name; }
    const std::string& getDescription() const override { return m_description; }
    int getManaCost() const override { return m_manaCost; }
    float getCooldown() const override { return m_cooldownDuration; }
    float getCurrentCooldown() const override { return m_currentCooldown; }

    void updateCooldown(float dt) override {
        if (m_currentCooldown > 0.0f) {
            m_currentCooldown = std::max(0.0f, m_currentCooldown - dt);
        }
    }

    void triggerCooldown() override {
        m_currentCooldown = m_cooldownDuration;
    }

    void resetCooldown() override {
        m_currentCooldown = 0.0f;
    }

    bool isReady() const override {
        return m_currentCooldown <= 0.0f;
    }

    const SpellCombination& getCombination() const override {
        return m_combination;
    }

    Color getThemeColor() const override {
        return m_themeColor;
    }

    bool canCast(const PlayerStats& stats) const override {
        return isReady() && stats.getMana() >= m_manaCost;
    }

protected:
    std::string m_name;
    std::string m_description;
    int m_manaCost;
    float m_cooldownDuration;
    float m_currentCooldown{0.0f};
    SpellCombination m_combination;
    Color m_themeColor;
};
