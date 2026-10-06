#include "SortingArena.hpp"
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../entities/SortingSerpent.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../quests/QuestManager.hpp"
#include "../core/InputManager.hpp"
#include <raymath.h>

SortingArena::SortingArena() {
    m_bounds = { 0, 0, 1800, 1200 };

    // Stone Pillars & Boundaries
    m_arenaPillars.push_back({ 300, 200, 60, 120 });
    m_arenaPillars.push_back({ 900, 200, 60, 120 });
    m_arenaPillars.push_back({ 300, 700, 60, 120 });
    m_arenaPillars.push_back({ 900, 700, 60, 120 });

    // Initial Crystal Puzzle Array
    m_crystalArray = { 35, 12, 48, 7, 26 };

    for (size_t i = 0; i < m_crystalArray.size(); ++i) {
        FloatingCrystal fc;
        fc.value = m_crystalArray[i];
        fc.position = { m_puzzleCenter.x - 120.0f + static_cast<float>(i) * 60.0f, m_puzzleCenter.y };
        fc.hoverOffset = static_cast<float>(i) * 0.5f;
        m_floatingCrystals.push_back(fc);
    }

    // Default Spell Organizer list
    m_sortedSpellDisplay = { "Dark Attack (Power: 70, Mana: 25)", "Fireball (Power: 50, Mana: 20)", "Heal (Power: 30, Mana: 15)", "Shield (Power: 20, Mana: 10)" };
}

bool SortingArena::isNearPuzzle(Vector2 pos) const {
    return Vector2Distance(pos, m_puzzleCenter) < 130.0f;
}

bool SortingArena::isNearSpellOrganizer(Vector2 pos) const {
    return Vector2Distance(pos, m_spellOrganizerPos) < 130.0f;
}

void SortingArena::triggerPuzzleSort(int methodIndex, AudioManager& audioManager, ParticleSystem& particles) {
    if (methodIndex == 0) {
        m_visualizer.setArray(m_crystalArray, m_bubbleSort);
        m_bubbleSort.sort(m_crystalArray);
    } else if (methodIndex == 1) {
        m_visualizer.setArray(m_crystalArray, m_mergeSort);
        m_mergeSort.sort(m_crystalArray);
    } else {
        m_visualizer.setArray(m_crystalArray, m_quickSort);
        m_quickSort.sort(m_crystalArray);
    }

    m_visualizer.startVisualization();
    m_puzzleSolved = true;

    // Update floating crystal values
    for (size_t i = 0; i < m_crystalArray.size() && i < m_floatingCrystals.size(); ++i) {
        m_floatingCrystals[i].value = m_crystalArray[i];
    }

    audioManager.playNote("C5", 0.5f);
    audioManager.playNote("G5", 0.6f);
    particles.emitBurst(m_puzzleCenter, GOLD, 30);
}

void SortingArena::cycleSpellSortingMethod() {
    m_spellSortCriteria = (m_spellSortCriteria + 1) % 3;
    sortSpells(m_spellSortCriteria);
}

void SortingArena::sortSpells(int criteriaIndex) {
    if (criteriaIndex == 0) {
        m_sortedSpellDisplay = { "Dark Attack (Power: 70)", "Fireball (Power: 50)", "Heal (Power: 30)", "Shield (Power: 20)" };
    } else if (criteriaIndex == 1) {
        m_sortedSpellDisplay = { "Shield (Mana: 10)", "Heal (Mana: 15)", "Fireball (Mana: 20)", "Dark Attack (Mana: 25)" };
    } else {
        m_sortedSpellDisplay = { "Fireball (Cooldown: 3s)", "Shield (Cooldown: 4s)", "Heal (Cooldown: 5s)", "Dark Attack (Cooldown: 6s)" };
    }
}

void SortingArena::resolveCollisions(Wizard& wizard) {
    Vector2 pos = wizard.getPosition();
    float radius = 20.0f;

    // Clamp to bounds
    if (pos.x - radius < m_bounds.x) pos.x = m_bounds.x + radius;
    if (pos.x + radius > m_bounds.x + m_bounds.width) pos.x = m_bounds.x + m_bounds.width - radius;
    if (pos.y - radius < m_bounds.y) pos.y = m_bounds.y + radius;
    if (pos.y + radius > m_bounds.y + m_bounds.height) pos.y = m_bounds.y + m_bounds.height - radius;

    wizard.setPosition(pos);
}

void SortingArena::update(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                         std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                         ParticleSystem& particles, QuestManager& questManager) {
    resolveCollisions(wizard);
    m_visualizer.update(deltaTime);

    // Update floating crystal animations
    for (auto& fc : m_floatingCrystals) {
        fc.hoverOffset += deltaTime * 2.0f;
    }

    // Spawn Sorting Serpent Boss if puzzle solved and boss not yet spawned
    if (m_puzzleSolved && !m_bossSpawned) {
        m_bossSpawned = true;
        enemies.push_back(std::make_unique<SortingSerpent>(Vector2{ 750.0f, 450.0f }));
        audioManager.playNote("C3", 0.7f);
        particles.emitBurst(Vector2{ 750.0f, 450.0f }, PURPLE, 40);
    }

    // Check boss defeated status
    if (m_bossSpawned && !m_bossDefeated) {
        bool serpentAlive = false;
        for (const auto& enemy : enemies) {
            if (enemy && enemy->getName() == "Sorting Serpent" && !enemy->isDead()) {
                serpentAlive = true;
                break;
            }
        }
        if (!serpentAlive) {
            m_bossDefeated = true;
            wizard.getStats().addExperience(150);
            questManager.onGuardianDefeated(); // Advancement
            particles.emitBurst(Vector2{ 750.0f, 450.0f }, GOLD, 50);
        }
    }

    // Input handlers for arena puzzle & visualizer speed
    if (inputManager) {
        if (inputManager->isKeyPressed(KEY_TAB)) {
            m_visualizer.cycleSpeed();
        }
        if (isNearPuzzle(wizard.getPosition()) && inputManager->isKeyPressed(KEY_E)) {
            triggerPuzzleSort(0, audioManager, particles); // Bubble Sort
        } else if (isNearPuzzle(wizard.getPosition()) && inputManager->isKeyPressed(KEY_R)) {
            triggerPuzzleSort(1, audioManager, particles); // Merge Sort
        } else if (isNearPuzzle(wizard.getPosition()) && inputManager->isKeyPressed(KEY_T)) {
            triggerPuzzleSort(2, audioManager, particles); // Quick Sort
        }

        if (isNearSpellOrganizer(wizard.getPosition()) && inputManager->isKeyPressed(KEY_E)) {
            cycleSpellSortingMethod();
            audioManager.playNote("E4", 0.4f);
        }
    }
}

void SortingArena::draw() const {
    // Draw Ancient Magical Arena Floor
    DrawRectangleRec(m_bounds, Color{ 25, 20, 35, 255 });

    // Sorting Lanes / Grid Lines
    for (int x = 200; x < 1600; x += 200) {
        DrawLine(x, 100, x, 1100, ColorAlpha(PURPLE, 0.15f));
    }

    // Pillars
    for (const auto& p : m_arenaPillars) {
        DrawRectangleRec(p, Color{ 45, 40, 60, 255 });
        DrawRectangleLinesEx(p, 2.0f, VIOLET);
    }

    // Crystal Puzzle Platform
    DrawCircleV(m_puzzleCenter, 90.0f, ColorAlpha(DARKBLUE, 0.6f));
    DrawCircleLinesV(m_puzzleCenter, 92.0f, GOLD);
    DrawText("CRYSTAL ARRAY ALGORITHM ALTAR", m_puzzleCenter.x - 130, m_puzzleCenter.y - 120, 16, GOLD);
    DrawText("[E] Bubble  |  [R] Merge  |  [T] Quick", m_puzzleCenter.x - 120, m_puzzleCenter.y + 100, 15, SKYBLUE);

    // Render Floating Crystals
    for (const auto& fc : m_floatingCrystals) {
        Vector2 drawPos = { fc.position.x, fc.position.y + sinf(fc.hoverOffset) * 8.0f };
        Color cColor = m_puzzleSolved ? GOLD : SKYBLUE;
        DrawCircleV(drawPos, 18.0f, cColor);
        DrawCircleLinesV(drawPos, 20.0f, WHITE);
        DrawText(TextFormat("%d", fc.value), drawPos.x - 8, drawPos.y - 6, 14, DARKBLUE);
    }

    // Spell Organizer Altar
    DrawCircleV(m_spellOrganizerPos, 70.0f, ColorAlpha(DARKBLUE, 0.6f));
    DrawCircleLinesV(m_spellOrganizerPos, 72.0f, LIME);
    DrawText("SPELL SORTING ALTAR", m_spellOrganizerPos.x - 80, m_spellOrganizerPos.y - 90, 16, LIME);
    DrawText("[E] Cycle Criteria (Power/Mana/CD)", m_spellOrganizerPos.x - 110, m_spellOrganizerPos.y + 75, 14, RAYWHITE);

    int displayY = m_spellOrganizerPos.y - 40;
    for (const auto& str : m_sortedSpellDisplay) {
        DrawText(str.c_str(), m_spellOrganizerPos.x - 100, displayY, 13, LIGHTGRAY);
        displayY += 18;
    }

    // Draw Algorithm Visualizer at bottom of screen
    m_visualizer.draw(400, 780, 1000, 220);
}
