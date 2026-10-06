#include "ChainedCave.hpp"
#include "../player/Wizard.hpp"
#include "../core/InputManager.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../quests/QuestManager.hpp"
#include "../entities/ShadowBat.hpp"
#include "../entities/StoneCrawler.hpp"
#include "../entities/ChainGuardian.hpp"
#include <raymath.h>
#include <cmath>

ChainedCave::ChainedCave()
    : m_bounds{0.0f, 0.0f, 1800.0f, 1400.0f} {
    // Cave boundaries / walls
    m_walls.push_back(Rectangle{50.0f, 50.0f, 1700.0f, 40.0f});     // Top
    m_walls.push_back(Rectangle{50.0f, 1310.0f, 1700.0f, 40.0f});   // Bottom
    m_walls.push_back(Rectangle{50.0f, 50.0f, 40.0f, 1300.0f});     // Left
    m_walls.push_back(Rectangle{1710.0f, 50.0f, 40.0f, 1300.0f});   // Right

    // Interior chasm pillars & walls
    m_walls.push_back(Rectangle{350.0f, 200.0f, 80.0f, 160.0f});
    m_walls.push_back(Rectangle{950.0f, 800.0f, 100.0f, 240.0f});
    m_walls.push_back(Rectangle{1350.0f, 300.0f, 120.0f, 180.0f});

    // Subterranean glowing pools
    m_waterPools.push_back({520.0f, 850.0f});
    m_waterPools.push_back({1220.0f, 360.0f});
    m_waterPools.push_back({780.0f, 600.0f});

    // Initialize Linked List with initial 2 crystal nodes
    m_crystalChain.insertBack({1, "Alpha", SKYBLUE, {460.0f, 380.0f}});
    m_crystalChain.insertBack({2, "Beta", PURPLE, {610.0f, 340.0f}});

    // Initialize Queue with 4 wave enemies for the Echo Gate
    m_enemyQueue.enqueue({"bat", {1050.0f, 450.0f}});
    m_enemyQueue.enqueue({"crawler", {1070.0f, 450.0f}});
    m_enemyQueue.enqueue({"bat", {1050.0f, 450.0f}});
    m_enemyQueue.enqueue({"crawler", {1080.0f, 450.0f}});
}

bool ChainedCave::isNearPuzzle(Vector2 pos) const {
    return Vector2Distance(pos, m_puzzleCenter) < 320.0f;
}

bool ChainedCave::isNearGate(Vector2 pos) const {
    return Vector2Distance(pos, m_gatePos) < 220.0f;
}

bool ChainedCave::isNearExit(Vector2 pos) const {
    return Vector2Distance(pos, m_exitPos) < 80.0f;
}

void ChainedCave::insertCrystal(AudioManager& audioManager) {
    if (m_crystalChain.size() >= 4) {
        m_listVisualizer.setMessage("Chain is full (4 nodes connected).", 3.0f);
        return;
    }

    size_t curSize = m_crystalChain.size();
    if (curSize == 2) {
        m_crystalChain.insertBack({3, "Gamma", GOLD, {760.0f, 380.0f}});
    } else if (curSize == 3) {
        m_crystalChain.insertBack({4, "Delta", LIME, {910.0f, 340.0f}});
    }

    audioManager.playSound("linked_list_insert");
    m_listVisualizer.setMessage("Crystal added to the end. That's like inserting at the tail.", 4.0f);
}

void ChainedCave::removeCrystal(AudioManager& audioManager) {
    if (m_crystalChain.size() <= 1) {
        m_listVisualizer.setMessage("Cannot remove head node from chain.", 3.0f);
        return;
    }

    m_crystalChain.removeBack();
    audioManager.playSound("chain_move");
    m_listVisualizer.setMessage("Crystal removed from the end. That's like removing from the tail.", 4.0f);
}

void ChainedCave::reverseCrystalChain(AudioManager& audioManager) {
    m_crystalChain.reverse();
    m_isReversed = !m_isReversed;
    audioManager.playSound("linked_list_reverse");

    if (m_crystalChain.size() == 4 && m_isReversed) {
        m_puzzleSolved = true;
        m_listVisualizer.setMessage("The chain is reversed and resonates in perfect harmony! Bridge unlocked!", 5.0f);
        audioManager.playSound("crystal_restore");
    } else {
        m_listVisualizer.setMessage("The chain is now reversed. Each node points to the previous node.", 4.0f);
    }
}

void ChainedCave::spawnQueuedEnemy(std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_enemyQueue.empty()) return;

    CaveEnemySpawn spawn = m_enemyQueue.front();
    m_enemyQueue.dequeue(); // FIFO Pop!

    audioManager.playSound("queue_dequeue");
    particles.emitBurst(spawn.pos, MAGENTA, 25, 120.0f);

    if (spawn.type == "bat") {
        enemies.push_back(std::make_unique<ShadowBat>(spawn.pos));
    } else {
        enemies.push_back(std::make_unique<StoneCrawler>(spawn.pos));
    }

    m_queueVisualizer.setMessage("Enemy dequeued from Echo Gate (FIFO dispatch).", 3.0f);
}

void ChainedCave::resolveCollisions(Wizard& wizard) {
    Vector2 pPos = wizard.getPosition();

    for (const auto& wall : m_walls) {
        if (CheckCollisionCircleRec(pPos, 20.0f, wall)) {
            Vector2 wCenter = {wall.x + wall.width / 2.0f, wall.y + wall.height / 2.0f};
            Vector2 dir = Vector2Normalize(Vector2Subtract(pPos, wCenter));
            wizard.setPosition(Vector2Add(pPos, Vector2Scale(dir, 3.5f)));
        }
    }
}

void ChainedCave::update(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                         std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                         ParticleSystem& particles, QuestManager& questManager) {
    resolveCollisions(wizard);

    m_listVisualizer.update(deltaTime);
    m_queueVisualizer.update(deltaTime);

    Vector2 pPos = wizard.getPosition();

    // 1. Linked List Puzzle controls when near puzzle
    if (isNearPuzzle(pPos) && inputManager) {
        if (inputManager->isKeyPressed(KEY_Z)) {
            insertCrystal(audioManager);
        } else if (inputManager->isKeyPressed(KEY_X)) {
            removeCrystal(audioManager);
        } else if (inputManager->isKeyPressed(KEY_C)) {
            reverseCrystalChain(audioManager);
        }

        // If puzzle just solved, make Guardian vulnerable if present
        if (m_puzzleSolved) {
            for (auto& e : enemies) {
                auto* g = dynamic_cast<ChainGuardian*>(e.get());
                if (g && g->isShielded()) {
                    g->breakChains(audioManager, particles);
                }
            }
        }
    }

    // 2. Echo Gate Queue wave dispatch
    if (isNearGate(pPos) && !m_gateActive && !m_gateCleared) {
        m_gateActive = true;
        m_spawnIntervalTimer = 1.0f;
        audioManager.playSound("queue_enqueue");
    }

    if (m_gateActive && !m_gateCleared) {
        m_spawnIntervalTimer += deltaTime;
        if (m_spawnIntervalTimer >= 3.0f && !m_enemyQueue.empty()) {
            m_spawnIntervalTimer = 0.0f;
            spawnQueuedEnemy(enemies, audioManager, particles);
        }

        if (m_enemyQueue.empty()) {
            // Check if all spawned bats/crawlers are defeated
            bool anyAlive = false;
            for (const auto& e : enemies) {
                if (e && !e->isDead() && dynamic_cast<ChainGuardian*>(e.get()) == nullptr) {
                    anyAlive = true;
                    break;
                }
            }
            if (!anyAlive) {
                m_gateCleared = true;
                m_gateActive = false;
                audioManager.playSound("level_up");
                m_queueVisualizer.setMessage("Queue complete! First in, first out. That is FIFO.", 6.0f);

                // Spawn Chain Guardian in the inner sanctum if not yet spawned
                if (!m_guardianSpawned) {
                    m_guardianSpawned = true;
                    auto guardian = std::make_unique<ChainGuardian>(Vector2{1250.0f, 720.0f});
                    if (m_puzzleSolved) {
                        guardian->breakChains(audioManager, particles);
                    }
                    enemies.push_back(std::move(guardian));
                    audioManager.playSound("guardian_roar");
                }
            }
        }
    }

    // 3. Subterranean Harmony Crystal Interaction
    float distToCrystal = Vector2Distance(pPos, m_caveCrystalPos);
    if (distToCrystal < 85.0f && !m_caveCrystalActivated && inputManager && inputManager->isKeyPressed(KEY_E)) {
        // Must defeat guardian first
        bool guardianAlive = false;
        for (const auto& e : enemies) {
            auto* g = dynamic_cast<ChainGuardian*>(e.get());
            if (g && !g->isDead()) {
                guardianAlive = true;
                break;
            }
        }

        if (guardianAlive) {
            m_listVisualizer.setMessage("Defeat the Chain Guardian to cleanse the subterranean crystal!", 4.0f);
        } else {
            m_caveCrystalActivated = true;
            audioManager.playSound("crystal_restore");
            particles.emitCrystalCleanse(m_caveCrystalPos);
            wizard.getStats().restoreHealth(100);
            wizard.getStats().restoreMana(100);
            wizard.getStats().addExperience(250);
            wizard.getStats().setDSAMastery(wizard.getStats().getDSAMastery() + 25.0f);
            questManager.onCrystalRestored();
            m_listVisualizer.setMessage("Subterranean Harmony Crystal Restored! The Chained Cave is at peace.", 6.0f);
        }
    }
}

void ChainedCave::draw() const {
    // 1. Cave Slate Floor
    DrawRectangleRec(m_bounds, Color{18, 14, 28, 255});

    // 2. Subterranean Glowing Water Pools
    float time = static_cast<float>(GetTime());
    for (size_t i = 0; i < m_waterPools.size(); ++i) {
        Vector2 pool = m_waterPools[i];
        float wave = std::sin(time * 3.0f + i) * 3.0f;
        DrawCircleV(pool, 75.0f + wave, Color{10, 80, 110, 180});
        DrawCircleV(pool, 60.0f, Color{20, 140, 170, 120});
        DrawCircleLines(static_cast<int>(pool.x), static_cast<int>(pool.y), 75.0f + wave, Color{40, 190, 220, 140});
    }

    // 3. Cave Walls
    for (const auto& wall : m_walls) {
        DrawRectangleRec(wall, Color{35, 30, 48, 255});
        DrawRectangle(static_cast<int>(wall.x + 4), static_cast<int>(wall.y + 4),
                      static_cast<int>(wall.width - 8), static_cast<int>(wall.height - 8), Color{25, 20, 35, 255});
    }

    // 4. Broken Stone Bridge (reconnects with radiant golden beams when puzzle solved!)
    Rectangle chasm = {960.0f, 620.0f, 180.0f, 80.0f};
    DrawRectangleRec(chasm, Color{8, 6, 12, 255});
    if (m_puzzleSolved) {
        // Radiant golden energy bridge
        DrawRectangle(static_cast<int>(chasm.x), static_cast<int>(chasm.y + 20), static_cast<int>(chasm.width), 40, Color{240, 200, 60, 160});
        for (int b = 0; b < 6; ++b) {
            float bx = chasm.x + b * 30.0f + 10.0f;
            DrawLineEx({bx, chasm.y + 20.0f}, {bx, chasm.y + 60.0f}, 3.0f, GOLD);
        }
    } else {
        DrawText("[BROKEN CHASM - REVERSE CRYSTAL CHAIN]", static_cast<int>(chasm.x - 20), static_cast<int>(chasm.y + 30), 10, Color{180, 150, 200, 200});
    }

    // 5. Linked List Crystal Chain Pedestals & Connecting Energy Beams
    std::vector<CrystalNodeData> chain = m_crystalChain.toVector();
    for (size_t i = 0; i < chain.size(); ++i) {
        Vector2 pos = chain[i].worldPos;

        // Pedestal
        DrawEllipse(static_cast<int>(pos.x), static_cast<int>(pos.y + 16), 20.0f, 7.0f, Color{0, 0, 0, 120});
        DrawRectangle(static_cast<int>(pos.x - 10), static_cast<int>(pos.y), 20, 20, Color{60, 55, 75, 255});

        // Floating Crystal
        float floatY = std::sin(time * 3.5f + i) * 6.0f;
        Vector2 crystalPos = {pos.x, pos.y - 12.0f + floatY};
        DrawPoly(crystalPos, 6, 15.0f, 30.0f, chain[i].color);
        DrawPoly(crystalPos, 6, 7.0f, 30.0f, WHITE);

        // Label
        DrawText(chain[i].name.c_str(), static_cast<int>(pos.x - 14), static_cast<int>(pos.y + 24), 10, chain[i].color);

        // Connecting Linked List Pointer Beam
        if (i + 1 < chain.size()) {
            Vector2 nextPos = {chain[i + 1].worldPos.x, chain[i + 1].worldPos.y - 12.0f + floatY};
            DrawLineEx(crystalPos, nextPos, 3.0f, SKYBLUE);
            DrawLineEx(crystalPos, nextPos, 1.0f, WHITE);

            // Midpoint pointer marker
            Vector2 mid = Vector2Scale(Vector2Add(crystalPos, nextPos), 0.5f);
            DrawCircleV(mid, 3.5f, m_isReversed ? GOLD : SKYBLUE);
        }
    }

    // 6. Echo Gate Archway
    DrawCircleLines(static_cast<int>(m_gatePos.x), static_cast<int>(m_gatePos.y), 45.0f, m_gateCleared ? LIME : PURPLE);
    DrawCircle(static_cast<int>(m_gatePos.x), static_cast<int>(m_gatePos.y), 32.0f, Color{25, 10, 35, 220});
    DrawText("ECHO GATE", static_cast<int>(m_gatePos.x - 30), static_cast<int>(m_gatePos.y - 58), 10, GOLD);

    // 7. Subterranean Harmony Crystal
    float cFloat = std::sin(time * 2.5f) * 6.0f;
    Color cCol = m_caveCrystalActivated ? SKYBLUE : Color{0, 100, 160, 255};
    DrawCircleV(m_caveCrystalPos, 28.0f, cCol);
    DrawCircleV({m_caveCrystalPos.x, m_caveCrystalPos.y + cFloat}, 15.0f, WHITE);
    DrawText("HARMONY CRYSTAL", static_cast<int>(m_caveCrystalPos.x - 52), static_cast<int>(m_caveCrystalPos.y - 42), 10, cCol);

    // 8. Cave Exit Portal back to Forest
    DrawCircleLines(static_cast<int>(m_exitPos.x), static_cast<int>(m_exitPos.y), 36.0f, Color{80, 220, 120, 255});
    DrawText("[EXIT TO FOREST (E)]", static_cast<int>(m_exitPos.x - 50), static_cast<int>(m_exitPos.y - 48), 10, LIME);
}
