#include "World.hpp"
#include "../player/Wizard.hpp"
#include "../core/InputManager.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../quests/QuestManager.hpp"
#include <raymath.h>
#include <cmath>

World::World() : m_bounds{0.0f, 0.0f, 2000.0f, 2000.0f} {
    // Populate Forest of Awakening
    // Corrupted grove trees
    m_trees.push_back({{350.0f, 400.0f}, 42.0f, true});
    m_trees.push_back({{520.0f, 360.0f}, 38.0f, true});
    m_trees.push_back({{360.0f, 530.0f}, 48.0f, true});
    m_trees.push_back({{530.0f, 520.0f}, 44.0f, true});
    
    // Healthy trees
    m_trees.push_back({{800.0f, 800.0f}, 45.0f, false});
    m_trees.push_back({{950.0f, 750.0f}, 40.0f, false});
    m_trees.push_back({{1200.0f, 900.0f}, 55.0f, false});
    m_trees.push_back({{700.0f, 1200.0f}, 48.0f, false});
    m_trees.push_back({{400.0f, 1100.0f}, 50.0f, false});
    
    // Rocks
    m_rocks.push_back({{700.0f, 200.0f, 80.0f, 60.0f}});
    m_rocks.push_back({{1000.0f, 1500.0f, 120.0f, 90.0f}});
    m_rocks.push_back({{250.0f, 320.0f, 60.0f, 50.0f}});

    // Harmony Crystal in the grove clearing
    m_crystal = {{620.0f, 460.0f}, 26.0f, false};
}

void World::update(Wizard& wizard, const InputManager* inputManager) {
    resolveCollisions(wizard);
    
    float dt = GetFrameTime();
    if (m_messageTimer > 0.0f) {
        m_messageTimer -= dt;
    } else {
        m_interactionMessage.clear();
    }
    
    float distToCrystal = Vector2Distance(wizard.getPosition(), m_crystal.position);
    m_showInteractionPrompt = (distToCrystal < 80.0f && !m_crystal.activated);
    
    if (m_showInteractionPrompt && inputManager && inputManager->isKeyPressed(KEY_E)) {
        m_crystal.activated = true;
        m_groveRestored = true;
        for (auto& tree : m_trees) {
            tree.corrupted = false;
        }
        m_interactionMessage = "The crystal remembers the first melody... The forest awakens!";
        m_messageTimer = 4.0f;
        wizard.getStats().setDSAMastery(wizard.getStats().getDSAMastery() + 10.0f);
    }
}

void World::update(Wizard& wizard, const InputManager* inputManager,
                   AudioManager& audioManager, ParticleSystem& particles,
                   QuestManager& questManager) {
    resolveCollisions(wizard);
    
    float dt = GetFrameTime();
    if (m_messageTimer > 0.0f) {
        m_messageTimer -= dt;
    } else {
        m_interactionMessage.clear();
    }

    if (m_groveRestored && m_restorationAlpha < 1.0f) {
        m_restorationAlpha += dt * 0.8f;
        if (m_restorationAlpha > 1.0f) m_restorationAlpha = 1.0f;
    }
    
    float distToCrystal = Vector2Distance(wizard.getPosition(), m_crystal.position);
    m_showInteractionPrompt = (distToCrystal < 85.0f && !m_crystal.activated);
    
    if (m_showInteractionPrompt && inputManager && inputManager->isKeyPressed(KEY_E)) {
        Quest* activeQuest = questManager.getActiveQuest();
        if (activeQuest && activeQuest->getState() == QuestState::ACTIVE) {
            m_interactionMessage = "Defeat the Corrupted Grove Beast guarding the crystal first!";
            m_messageTimer = 3.5f;
            return;
        }

        // Cleanse and restore!
        m_crystal.activated = true;
        m_groveRestored = true;

        for (auto& tree : m_trees) {
            tree.corrupted = false;
        }

        audioManager.playSound("crystal_restore");
        particles.emitCrystalCleanse(m_crystal.position);

        // Player rewards
        wizard.getStats().restoreHealth(100);
        wizard.getStats().restoreMana(100);
        wizard.getStats().addExperience(150);
        wizard.getStats().setDSAMastery(wizard.getStats().getDSAMastery() + 15.0f);

        questManager.onCrystalRestored();

        m_interactionMessage = "Harmonic Resonance Restored! The grove flourishes!";
        m_messageTimer = 5.0f;
    }
}

void World::resolveCollisions(Wizard& wizard) {
    Vector2 pPos = wizard.getPosition();
    
    for (const auto& rock : m_rocks) {
        if (CheckCollisionCircleRec(pPos, 20.0f, rock.bounds)) {
            Vector2 rockCenter = {rock.bounds.x + rock.bounds.width/2, rock.bounds.y + rock.bounds.height/2};
            Vector2 dir = Vector2Normalize(Vector2Subtract(pPos, rockCenter));
            wizard.setPosition(Vector2Add(pPos, Vector2Scale(dir, 3.0f)));
        }
    }
    
    for (const auto& tree : m_trees) {
        if (CheckCollisionCircles(pPos, 20.0f, tree.position, tree.radius - 12.0f)) {
            Vector2 dir = Vector2Normalize(Vector2Subtract(pPos, tree.position));
            wizard.setPosition(Vector2Add(pPos, Vector2Scale(dir, 3.0f)));
        }
    }
}

void World::draw() const {
    // Base grass
    DrawRectangleRec(m_bounds, Color{34, 139, 34, 255}); // Forest green
    
    Vector2 groveCenter = {450.0f, 450.0f};

    if (!m_groveRestored) {
        // Dark corrupted mist circle over grove
        DrawCircleGradient(groveCenter, 380.0f, Color{25, 10, 35, 210}, Color{25, 10, 35, 0});
    } else {
        // Restored grove: radiant blooming sunlit clearing
        DrawCircleGradient(groveCenter, 380.0f, Color{60, 180, 75, 180}, Color{34, 139, 34, 0});

        // Wildflowers blooming in cleansed grove
        for (int i = 0; i < 12; ++i) {
            float fx = groveCenter.x + std::sin(i * 1.7f) * 160.0f;
            float fy = groveCenter.y + std::cos(i * 1.7f) * 160.0f;
            DrawCircle(static_cast<int>(fx), static_cast<int>(fy), 3.0f, (i % 2 == 0) ? GOLD : PINK);
        }
    }
    
    // Draw Rocks
    for (const auto& rock : m_rocks) {
        DrawRectangleRec(rock.bounds, GRAY);
        DrawRectangle(static_cast<int>(rock.bounds.x + 5), static_cast<int>(rock.bounds.y + 5), 
                      static_cast<int>(rock.bounds.width - 10), static_cast<int>(rock.bounds.height - 10), DARKGRAY);
    }
    
    // Draw Trees
    for (const auto& tree : m_trees) {
        // Shadow
        DrawEllipse(static_cast<int>(tree.position.x), static_cast<int>(tree.position.y + tree.radius/2), 
                    tree.radius * 1.2f, tree.radius * 0.6f, Color{0, 0, 0, 90});
        
        // Trunk
        DrawRectangle(static_cast<int>(tree.position.x - tree.radius*0.2f), 
                      static_cast<int>(tree.position.y), 
                      static_cast<int>(tree.radius*0.4f), 
                      static_cast<int>(tree.radius), BROWN);
                      
        // Foliage
        Color leafColor;
        if (tree.corrupted) {
            leafColor = Color{45, 20, 65, 255};
        } else {
            leafColor = Color{20, 100, 35, 255};
        }

        DrawCircleV({tree.position.x, tree.position.y - tree.radius*0.5f}, tree.radius, leafColor);

        if (tree.corrupted) {
            // Corruption purple embers
            DrawCircle(static_cast<int>(tree.position.x - 10), static_cast<int>(tree.position.y - tree.radius), 3.0f, PURPLE);
            DrawCircle(static_cast<int>(tree.position.x + 15), static_cast<int>(tree.position.y - tree.radius + 10), 4.0f, MAGENTA);
        } else if (m_groveRestored) {
            // Cleansed golden blossom sparkles
            DrawCircle(static_cast<int>(tree.position.x - 8), static_cast<int>(tree.position.y - tree.radius - 2), 2.5f, GOLD);
            DrawCircle(static_cast<int>(tree.position.x + 10), static_cast<int>(tree.position.y - tree.radius + 8), 2.0f, LIME);
        }
    }
    
    // Draw Harmony Crystal
    float time = static_cast<float>(GetTime());
    Color crystalCore = m_crystal.activated ? SKYBLUE : Color{0, 110, 160, 255};
    float pulse = std::sin(time * 3.5f) * 3.0f;

    // Outer glow
    DrawCircleV(m_crystal.position, m_crystal.radius + 6.0f + pulse, 
                m_crystal.activated ? Color{0, 200, 255, 60} : Color{0, 100, 150, 40});
    // Main crystal body
    DrawCircleV(m_crystal.position, m_crystal.radius, crystalCore);
    DrawCircleV(m_crystal.position, m_crystal.radius * 0.55f, WHITE);

    if (m_crystal.activated) {
        // Rotating harmonic rays
        for (int i = 0; i < 4; ++i) {
            float angle = time * 2.0f + i * (3.14159f / 2.0f);
            Vector2 rayEnd = {
                m_crystal.position.x + std::cos(angle) * (m_crystal.radius + 18.0f),
                m_crystal.position.y + std::sin(angle) * (m_crystal.radius + 18.0f)
            };
            DrawLineEx(m_crystal.position, rayEnd, 2.5f, Color{180, 240, 255, 200});
        }
    } else {
        // Floating dormant particle
        float offsetY = std::sin(time * 2.5f) * 8.0f;
        DrawCircle(static_cast<int>(m_crystal.position.x), 
                   static_cast<int>(m_crystal.position.y - m_crystal.radius - 12.0f + offsetY), 
                   3.5f, SKYBLUE);
    }
    
    // Interaction prompt
    if (m_showInteractionPrompt) {
        const char* prompt = "Press [E] to Cleanse Harmony Crystal";
        int tw = MeasureText(prompt, 16);
        DrawRectangle(static_cast<int>(m_crystal.position.x - tw / 2 - 8),
                      static_cast<int>(m_crystal.position.y - m_crystal.radius - 38),
                      tw + 16, 24, Color{0, 0, 0, 180});
        DrawText(prompt, 
                 static_cast<int>(m_crystal.position.x - tw / 2), 
                 static_cast<int>(m_crystal.position.y - m_crystal.radius - 34), 
                 16, SKYBLUE);
    }
}
