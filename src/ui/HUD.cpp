#include "HUD.hpp"
#include "../player/Wizard.hpp"
#include "../world/World.hpp"
#include "../spells/AudioSpellSystem.hpp"
#include "../combat/CombatSystem.hpp"
#include "../quests/QuestManager.hpp"
#include "../entities/Enemy.hpp"
#include "DSAMasteryPanel.hpp"
#include <raylib.h>
#include <cstdio>
#include <cmath>

HUD::HUD() {}

void HUD::drawProgressBar(int x, int y, int width, int height, float percentage, Color fill, Color bg, const char* label) const {
    DrawRectangleRounded({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)}, 0.5f, 4, bg);
    
    if (percentage > 0.0f) {
        float fillW = static_cast<float>(width) * percentage;
        DrawRectangleRounded({static_cast<float>(x), static_cast<float>(y), fillW, static_cast<float>(height)}, 0.5f, 4, fill);
    }
    
    if (label) {
        DrawText(label, x + 8, y + 2, 10, WHITE);
    }
}

void HUD::drawTextCentered(const char* text, int y, int fontSize, Color color) const {
    int w = MeasureText(text, fontSize);
    DrawText(text, GetScreenWidth() / 2 - w / 2, y, fontSize, color);
}

void HUD::drawExplorationHUD(const PlayerStats& stats, const World& world) const {
    // Backward compatibility if needed
}

void HUD::drawHUD(const Wizard& wizard, const World& world, const ChainedCave* cave,
                  const AudioSpellSystem& spellSystem, const CombatSystem& combatSystem,
                  const QuestManager& questManager,
                  const std::vector<std::unique_ptr<Enemy>>& enemies,
                  const DSAMasteryPanel* masteryPanel) const {
    
    int sw = GetScreenWidth();
    
    // Top-left: Player Status
    DrawRectangleRounded({10.0f, 10.0f, 260.0f, 100.0f}, 0.1f, 8, Color{20, 25, 40, 220});
    DrawRectangleRoundedLinesEx({10.0f, 10.0f, 260.0f, 100.0f}, 0.1f, 8, 2.0f, Color{70, 90, 120, 255});

    DrawText("🧙 WIZARD", 24, 20, 16, GOLD);
    
    char lvlTxt[32];
    snprintf(lvlTxt, sizeof(lvlTxt), "LV %d", wizard.getStats().getLevel());
    DrawText(lvlTxt, 210, 20, 16, SKYBLUE);
    
    float hpPct = (float)wizard.getStats().getHealth() / (float)wizard.getStats().getMaxHealth();
    float mpPct = (float)wizard.getStats().getMana() / (float)wizard.getStats().getMaxMana();
    
    DrawText("HP", 24, 45, 12, LIGHTGRAY);
    drawProgressBar(60, 45, 190, 12, hpPct, Color{220, 40, 60, 255}, Color{60, 20, 30, 255}, nullptr);
    
    DrawText("MANA", 24, 65, 12, LIGHTGRAY);
    drawProgressBar(60, 65, 190, 12, mpPct, Color{40, 150, 220, 255}, Color{20, 40, 60, 255}, nullptr);
    
    float xpPct = (wizard.getStats().getLevel() < 5) ? ((float)wizard.getStats().getXP() / (wizard.getStats().getLevel() * 100)) : 1.0f;
    DrawText("XP", 24, 85, 10, LIGHTGRAY);
    drawProgressBar(60, 85, 190, 8, xpPct, GOLD, Color{60, 50, 20, 255}, nullptr);

    // Enemy Boss Bar
    drawEnemyBossBar(enemies);

    // Right Side: Quest Tracker
    drawQuestTracker(sw - 280, 10, questManager);

    // Bottom Area: Spellbook and Abilities
    drawSpellbook(10, GetScreenHeight() - 170, spellSystem, wizard);
    drawStackVisualizer(10, GetScreenHeight() - 250, spellSystem);
}

void HUD::drawEnemyBossBar(const std::vector<std::unique_ptr<Enemy>>& enemies) const {
    if (enemies.empty()) return;
    
    int sw = GetScreenWidth();
    int barW = 220;
    int barX = sw / 2 - barW / 2;
    int barY = 25;
    
    for (const auto& e : enemies) {
        if (e->isDead()) continue;
        
        float hpPct = (float)e->getHealth() / (float)e->getMaxHealth();
        
        DrawRectangleRounded({static_cast<float>(barX - 10), static_cast<float>(barY - 20), static_cast<float>(barW + 20), 45.0f}, 0.2f, 4, Color{20, 20, 30, 200});
        DrawRectangleRoundedLinesEx({static_cast<float>(barX - 10), static_cast<float>(barY - 20), static_cast<float>(barW + 20), 45.0f}, 0.2f, 4, 1.5f, Color{150, 40, 40, 255});
        
        drawTextCentered(e->getName().c_str(), barY - 12, 14, GOLD);
        drawProgressBar(barX, barY + 8, barW, 10, hpPct, RED, DARKGRAY, nullptr);
        
        break; // Only draw one for now (Boss)
    }
}

void HUD::drawQuestTracker(int x, int y, const QuestManager& questManager) const {
    int w = 260;
    int h = 120;
    
    DrawRectangleRounded({static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)}, 0.1f, 8, Color{20, 25, 40, 220});
    DrawRectangleRoundedLinesEx({static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)}, 0.1f, 8, 2.0f, Color{200, 160, 50, 150});
    
    DrawText("✦ CURRENT QUEST", x + 16, y + 16, 14, GOLD);
    DrawLine(x + 16, y + 36, x + w - 16, y + 36, Color{200, 160, 50, 100});
    
    const Quest* q = questManager.getActiveQuest();
    int oy = y + 45;
    if (!q) {
        DrawText("No active quests.", x + 16, oy, 12, LIGHTGRAY);
    } else {
        DrawText(q->getTitle().c_str(), x + 16, oy, 14, WHITE);
        oy += 20;
        DrawText(q->getObjectiveText().c_str(), x + 24, oy, 12, SKYBLUE);
        oy += 20;
    }
}

void HUD::drawSpellbook(int x, int y, const AudioSpellSystem& spellSystem, const Wizard& wizard) const {
    int w = 260;
    int h = 150;
    
    DrawRectangleRounded({static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)}, 0.1f, 8, Color{20, 25, 40, 220});
    DrawRectangleRoundedLinesEx({static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)}, 0.1f, 8, 2.0f, Color{70, 90, 120, 255});
    
    DrawText("SPELLBOOK", x + 16, y + 16, 14, GOLD);
    
    // Notes with glowing state
    const char* notes[] = {"C", "D", "E", "F", "G", "A", "B"};
    auto seq = spellSystem.getActiveSequence();
    float time = GetTime();
    
    for (int i = 0; i < 7; ++i) {
        int nx = x + 20 + i * 30;
        int ny = y + 45;
        
        bool isActive = false;
        for (const auto& note : seq) {
            if (note.name == notes[i]) {
                isActive = true;
                break;
            }
        }
        
        if (isActive) {
            float pulse = std::sin(time * 10.0f) * 3.0f;
            DrawCircle(nx + 4, ny + 20, 8.0f + pulse, Color{100, 200, 255, 100});
            DrawText(notes[i], nx, ny, 14, GOLD);
            DrawCircle(nx + 4, ny + 20, 6, SKYBLUE);
        } else {
            DrawText(notes[i], nx, ny, 12, GRAY);
            DrawCircle(nx + 4, ny + 20, 6, DARKGRAY);
        }
    }
    
    DrawLine(x + 16, y + 85, x + w - 16, y + 85, Color{70, 90, 120, 150});
    DrawText("CURRENT MELODY", x + 16, y + 100, 10, SKYBLUE);
    
    std::string mel = "";
    if (seq.empty()) {
        mel = "(No Melody)";
    } else {
        for (size_t i = 0; i < seq.size(); ++i) {
            mel += seq[i].name;
            if (i < seq.size() - 1) mel += " -> ";
        }
    }
    DrawText(mel.c_str(), x + 16, y + 120, 14, WHITE);
}

void HUD::drawStackVisualizer(int x, int y, const AudioSpellSystem& spellSystem) const {
    int w = 180;
    int h = 70;
    DrawRectangleRounded({static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)}, 0.1f, 8, Color{20, 25, 40, 200});
    DrawRectangleRoundedLinesEx({static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)}, 0.1f, 8, 1.5f, Color{70, 90, 120, 200});
    
    DrawText("✦ RECENT SPELLS", x + 12, y + 12, 10, GOLD);
    DrawText("[Q] Undo Last Spell", x + 12, y + 45, 10, SKYBLUE);
}

void HUD::drawMainMenu() const {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float time = GetTime();
    
    // Background magical gradient
    DrawRectangleGradientV(0, 0, sw, sh, Color{10, 15, 30, 255}, Color{30, 20, 60, 255});
    
    // Magical particles / fireflies
    for (int i = 0; i < 60; ++i) {
        float px = std::fmod((i * 142.4f) + (time * (10.0f + i)), (float)sw);
        float py = std::fmod((i * 321.4f) - (time * (15.0f + i % 5)), (float)sh);
        float alpha = (std::sin(time * 2.0f + i) + 1.0f) * 0.5f;
        Color c = (i % 3 == 0) ? GOLD : SKYBLUE;
        c.a = (unsigned char)(150 * alpha);
        DrawCircle(px, py, (i % 3 == 0) ? 2.5f : 1.5f, c);
    }
    
    drawTextCentered("AUDIO WIZARD", 150, 64, GOLD);
    drawTextCentered("Restore the Harmony", 230, 24, SKYBLUE);
    
    int bx = sw / 2 - 120;
    int by = 350;
    Rectangle playBtn = {static_cast<float>(bx), static_cast<float>(by), 240.0f, 50.0f};
    
    bool hovered = CheckCollisionPointRec(GetMousePosition(), playBtn);
    
    if (hovered) {
        DrawRectangleRounded(playBtn, 0.2f, 8, Color{40, 70, 140, 255});
        DrawRectangleRoundedLinesEx(playBtn, 0.2f, 8, 3.0f, GOLD);
        drawTextCentered("PLAY", by + 15, 22, GOLD);
    } else {
        DrawRectangleRounded(playBtn, 0.2f, 8, Color{30, 50, 100, 200});
        DrawRectangleRoundedLinesEx(playBtn, 0.2f, 8, 2.0f, SKYBLUE);
        drawTextCentered("PLAY", by + 15, 20, WHITE);
    }
    
    float pulse = std::sin(time * 3.0f) * 100.0f + 155.0f;
    DrawText("Click PLAY or Press [ENTER] to start", sw / 2 - 120, 440, 14, Color{200, 200, 200, (unsigned char)pulse});
}

void HUD::drawPauseMenu() const {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, 180});
    
    int pw = 360;
    int ph = 480;
    int px = sw / 2 - pw / 2;
    int py = sh / 2 - ph / 2;
    
    // Magical glow behind panel
    DrawCircleGradient({(float)sw / 2, (float)sh / 2}, 300.0f, Color{40, 30, 80, 120}, BLANK);
    
    DrawRectangleRounded({static_cast<float>(px), static_cast<float>(py), static_cast<float>(pw), static_cast<float>(ph)}, 0.1f, 8, Color{15, 20, 35, 240});
    DrawRectangleRoundedLinesEx({static_cast<float>(px), static_cast<float>(py), static_cast<float>(pw), static_cast<float>(ph)}, 0.1f, 8, 2.0f, GOLD);
    
    drawTextCentered("PAUSED", py + 30, 28, GOLD);
    DrawLine(px + 40, py + 70, px + pw - 40, py + 70, Color{255, 215, 0, 100});
    
    const char* options[] = {
        "RESUME  [ESC]",
        "SPELLBOOK",
        "DSA KNOWLEDGE  [M]",
        "QUESTS",
        "SETTINGS",
        "MAIN MENU"
    };
    
    int startY = py + 100;
    Vector2 mousePos = GetMousePosition();
    
    for (int i = 0; i < 6; ++i) {
        Rectangle btn = {static_cast<float>(px + 40), static_cast<float>(startY + i * 55), static_cast<float>(pw - 80), 40.0f};
        bool hovered = CheckCollisionPointRec(mousePos, btn);
        
        if (hovered) {
            DrawRectangleRounded(btn, 0.2f, 4, Color{40, 60, 100, 200});
            DrawRectangleRoundedLinesEx(btn, 0.2f, 4, 1.5f, SKYBLUE);
            drawTextCentered(options[i], startY + i * 55 + 10, 18, WHITE);
        } else {
            DrawRectangleRounded(btn, 0.2f, 4, Color{25, 35, 60, 150});
            drawTextCentered(options[i], startY + i * 55 + 12, 16, LIGHTGRAY);
        }
    }
}

void HUD::drawVictoryScreen() const {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, 200});
    drawTextCentered("VICTORY!", sh / 2 - 50, 60, GOLD);
    drawTextCentered("The harmony is restored.", sh / 2 + 30, 20, WHITE);
    drawTextCentered("Press [ENTER] to return", sh / 2 + 100, 14, LIGHTGRAY);
}

void HUD::drawGameOverScreen() const {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    DrawRectangle(0, 0, sw, sh, Color{50, 0, 0, 200});
    drawTextCentered("DEFEATED", sh / 2 - 50, 60, RED);
    drawTextCentered("The corruption consumes the land...", sh / 2 + 30, 20, WHITE);
    drawTextCentered("Press [ENTER] to restart", sh / 2 + 100, 14, LIGHTGRAY);
}
