#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>
#include "../dsa/DynamicArray.hpp"
#include "../dsa/SortingAlgorithms.hpp"
#include "../dsa/visualization/SortingVisualizer.hpp"

class Wizard;
class Enemy;
class AudioManager;
class ParticleSystem;
class QuestManager;

struct FloatingCrystal {
    Vector2 position;
    int value;
    float hoverOffset;
};

class SortingArena {
public:
    SortingArena();

    void update(float deltaTime, Wizard& wizard, const class InputManager* inputManager,
                std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                ParticleSystem& particles, QuestManager& questManager);

    void draw() const;

    Rectangle getBounds() const { return m_bounds; }
    Vector2 getEntrancePos() const { return m_entrancePos; }
    Vector2 getExitPos() const { return m_exitPos; }

    bool isPuzzleSolved() const { return m_puzzleSolved; }
    bool isBossDefeated() const { return m_bossDefeated; }

    void triggerPuzzleSort(int methodIndex, AudioManager& audioManager, ParticleSystem& particles);
    void cycleSpellSortingMethod();
    void sortSpells(int criteriaIndex);

    bool isNearPuzzle(Vector2 pos) const;
    bool isNearSpellOrganizer(Vector2 pos) const;

    const DynamicArray<int>& getCrystalArray() const { return m_crystalArray; }
    const SortingVisualizer& getVisualizer() const { return m_visualizer; }

private:
    void resolveCollisions(Wizard& wizard);

    Rectangle m_bounds;
    Vector2 m_entrancePos{200.0f, 650.0f};
    Vector2 m_exitPos{180.0f, 650.0f};

    std::vector<Rectangle> m_arenaPillars;
    std::vector<FloatingCrystal> m_floatingCrystals;

    // Interactive Crystal Sorting Puzzle
    DynamicArray<int> m_crystalArray;
    bool m_puzzleSolved{false};
    Vector2 m_puzzleCenter{600.0f, 350.0f};

    // Spell Organizer Altar
    Vector2 m_spellOrganizerPos{1100.0f, 350.0f};
    int m_spellSortCriteria{0}; // 0 = Power, 1 = Mana, 2 = Cooldown
    std::vector<std::string> m_sortedSpellDisplay;

    // Boss State
    bool m_bossSpawned{false};
    bool m_bossDefeated{false};

    BubbleSort m_bubbleSort;
    MergeSort m_mergeSort;
    QuickSort m_quickSort;
    SortingVisualizer m_visualizer;
};
