#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <memory>

class PlayerStats;
class Wizard;
class World;
class AudioSpellSystem;
class CombatSystem;
class QuestManager;
class Enemy;
class ChainedCave;
class DSAMasteryPanel;

class HUD {
public:
    HUD();

    // Backward-compatible Phase 2 HUD
    void drawExplorationHUD(const PlayerStats& stats, const World& world) const;

    // Full Phase 3/4 Combat, Exploration & DSA Gameplay HUD
    void drawHUD(const Wizard& wizard, const World& world, const ChainedCave* cave,
                 const AudioSpellSystem& spellSystem, const CombatSystem& combatSystem,
                 const QuestManager& questManager,
                 const std::vector<std::unique_ptr<Enemy>>& enemies,
                 const DSAMasteryPanel* masteryPanel = nullptr) const;

    void drawMainMenu() const;
    void drawPauseMenu() const;
    void drawVictoryScreen() const;
    void drawGameOverScreen() const;

private:
    void drawProgressBar(int x, int y, int width, int height, float percentage, Color fill, Color bg, const char* label) const;
    void drawTextCentered(const char* text, int y, int fontSize, Color color) const;
    void drawSpellbook(int x, int y, const AudioSpellSystem& spellSystem, const Wizard& wizard) const;
    void drawNoteSequence(int x, int y, const AudioSpellSystem& spellSystem) const;
    void drawStackVisualizer(int x, int y, const AudioSpellSystem& spellSystem) const;
    void drawQuestTracker(int x, int y, const QuestManager& questManager) const;
    void drawEnemyBossBar(const std::vector<std::unique_ptr<Enemy>>& enemies) const;
    void drawAbilitiesHUD(int x, int y, const Wizard& wizard) const;
};

