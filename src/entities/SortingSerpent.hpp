#pragma once

#include "Enemy.hpp"
#include "../dsa/DynamicArray.hpp"
#include "../dsa/SortingAlgorithms.hpp"
#include <vector>
#include <string>

enum class BossPhase {
    BUBBLE_PHASE = 1,
    MERGE_PHASE = 2,
    QUICK_PHASE = 3
};

struct SerpentSegment {
    int value{0};
    Vector2 position{0.0f, 0.0f};
    bool isPivot{false};
    bool isSorted{false};
};

class SortingSerpent : public Enemy {
public:
    SortingSerpent(Vector2 position = {750.0f, 450.0f});

    void update(float deltaTime) override {}
    void updateAI(float deltaTime, Wizard& wizard, const World& world, AudioManager& audioManager, ParticleSystem& particles) override;
    void draw() const override;

    void takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) override;
    void takeDamage(float damage);

    bool isDead() const override { return m_health <= 0; }
    bool isDefeated() const override { return m_health <= 0; }
    int getHealth() const override { return static_cast<int>(m_health); }
    int getMaxHealth() const override { return static_cast<int>(m_maxHealth); }
    int getXPReward() const override { return 450; }
    Rectangle getBounds() const override;
    const std::string& getName() const override { return m_name; }
    bool isInCombat() const override { return true; }

    bool isAlive() const { return m_health > 0.0f; }

    BossPhase getBossPhase() const { return m_phase; }
    const DynamicArray<int>& getSegmentArray() const { return m_segmentValues; }

    bool isVulnerable() const { return m_vulnerable; }
    float getVulnerableTimer() const { return m_vulnerableTimer; }

    // Interactive sorting actions triggered by player spell / arena puzzle
    bool applySortingStep(const std::string& method, AudioManager& audioManager, ParticleSystem& particles);

private:
    void updateSegmentPositions();
    void checkPhaseTransition(ParticleSystem& particles, AudioManager& audioManager);
    void triggerBossAttack(Wizard& wizard, ParticleSystem& particles, AudioManager& audioManager);

    std::string m_name{"Sorting Serpent"};
    float m_health{450.0f};
    float m_maxHealth{450.0f};

    BossPhase m_phase{BossPhase::BUBBLE_PHASE};
    DynamicArray<int> m_segmentValues;
    std::vector<SerpentSegment> m_segments;

    bool m_vulnerable{false};
    float m_vulnerableTimer{0.0f};
    float m_attackTimer{0.0f};

    BubbleSort m_bubbleSort;
    MergeSort m_mergeSort;
    QuickSort m_quickSort;
};
