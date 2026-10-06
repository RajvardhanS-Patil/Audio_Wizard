#include "Game.hpp"
#include "../entities/CorruptedCreature.hpp"
#include "../entities/ShadowBat.hpp"
#include "../entities/StoneCrawler.hpp"
#include "../entities/ChainGuardian.hpp"
#include "../entities/SortingSerpent.hpp"
#include "../entities/TreeGuardian.hpp"
#include <raymath.h>

Game::Game()
    : m_inputManager(std::make_unique<InputManager>()),
      m_audioManager(std::make_unique<AudioManager>()) {
  InitWindow(800, 600, "Audio Wizard - Phase 5: The Sorting Arena");
  SetTargetFPS(60);

  // Initialize Camera
  m_camera.target = m_wizard.getPosition();
  m_camera.offset = {800.0f / 2.0f, 600.0f / 2.0f};
  m_camera.rotation = 0.0f;
  m_camera.zoom = 1.0f;

  // Initialize Audio with sound synthesis and music
  m_audioManager->init();

  // Spawn the initial enemy: Corrupted Grove Beast
  m_enemies.push_back(std::make_unique<CorruptedCreature>(Vector2{480.0f, 440.0f}));
}

Game::~Game() {
  m_audioManager.reset(); // Close audio device before window
  CloseWindow();
}

void Game::restartGame() {
  m_wizard.setPosition({400.0f, 300.0f});
  m_wizard.getStats().restoreHealth(100);
  m_wizard.getStats().restoreMana(100);
  m_wizard.activateShield(0.0f, 0.0f);

  m_enemies.clear();
  m_currentZone = ZoneType::FOREST_OF_AWAKENING;
  m_enemies.push_back(std::make_unique<CorruptedCreature>(Vector2{480.0f, 440.0f}));

  m_combatSystem.resetCombo();
  m_victoryShown = false;
}

void Game::run() {
  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    processEvents();
    update(deltaTime);
    render();
  }
}

void Game::processEvents() { m_inputManager->update(); }

void Game::update(float deltaTime) {
  switch (m_currentState) {
  case GameState::MAIN_MENU:
    if (m_inputManager->isKeyPressed(KEY_ENTER) ||
        m_inputManager->isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      m_currentState = GameState::EXPLORATION;
    }
    break;

  case GameState::EXPLORATION:
    if (m_inputManager->isKeyPressed(KEY_ESCAPE)) {
      m_currentState = GameState::PAUSE;
    } else {
      // Toggle DSA Mastery Panel
      if (m_inputManager->isKeyPressed(KEY_M) || m_inputManager->isKeyPressed(KEY_TAB)) {
        m_masteryPanel.toggle();
      }
      m_masteryPanel.update(deltaTime, m_inputManager.get());

      // If DSA Mastery Panel is open, pause world updates or handle panel inputs
      if (m_masteryPanel.isVisible()) {
        break;
      }

      // Check Ability Inputs: [R] Chain Link, [T] Queue Surge
      if (m_inputManager->isKeyPressed(KEY_R)) {
        m_wizard.castChainLink(m_enemies, m_combatSystem.getParticles(), *m_audioManager);
      }
      if (m_inputManager->isKeyPressed(KEY_T)) {
        m_wizard.castQueueSurge(m_combatSystem, *m_audioManager);
      }

      Rectangle currentBounds = (m_currentZone == ZoneType::FOREST_OF_AWAKENING) ? m_world.getBounds() :
                                ((m_currentZone == ZoneType::CHAINED_CAVE) ? m_cave.getBounds() : 
                                ((m_currentZone == ZoneType::SORTING_ARENA) ? m_sortingArena.getBounds() : 
                                ((m_currentZone == ZoneType::TREE_OF_KNOWLEDGE) ? m_treeOfKnowledge.getBounds() : m_graphCity.getBounds())));

      // 1. Update Wizard
      m_wizard.update(deltaTime, m_inputManager.get(), currentBounds);

      // 2. Update Active Zone
      if (m_currentZone == ZoneType::FOREST_OF_AWAKENING) {
        m_world.update(m_wizard, m_inputManager.get(), *m_audioManager, 
                       m_combatSystem.getParticles(), m_questManager);

        // Zone transition: Step into Chained Cave portal
        Vector2 wPos = m_wizard.getPosition();
        if (wPos.x > 1500.0f && wPos.y < 300.0f) {
          if (m_inputManager->isKeyPressed(KEY_E)) {
            m_currentZone = ZoneType::CHAINED_CAVE;
            m_wizard.setPosition(m_cave.getEntrancePos());
            m_audioManager->playSound("portal_travel");
            m_questManager.onEnterCave();
          }
        }
      } else if (m_currentZone == ZoneType::CHAINED_CAVE) {
        // Chained Cave Zone Update
        m_cave.update(deltaTime, m_wizard, m_inputManager.get(), m_enemies, 
                      *m_audioManager, m_combatSystem.getParticles(), m_questManager);

        // Zone transition: Enter Sorting Arena portal near exit or key [F]
        if (m_inputManager->isKeyPressed(KEY_F)) {
          m_currentZone = ZoneType::SORTING_ARENA;
          m_wizard.setPosition(m_sortingArena.getEntrancePos());
          m_audioManager->playSound("portal_travel");
          m_questManager.onEnterSortingArena();
        } else if (m_cave.isNearExit(m_wizard.getPosition()) && m_inputManager->isKeyPressed(KEY_E)) {
          m_currentZone = ZoneType::FOREST_OF_AWAKENING;
          m_wizard.setPosition({1520.0f, 280.0f});
          m_audioManager->playSound("portal_travel");
        }
      } else if (m_currentZone == ZoneType::SORTING_ARENA) {
        // Sorting Arena Zone Update
        m_sortingArena.update(deltaTime, m_wizard, m_inputManager.get(), m_enemies,
                              *m_audioManager, m_combatSystem.getParticles(), m_questManager);

        // Zone transition back to Chained Cave
        if (m_wizard.getPosition().x < 190.0f && m_inputManager->isKeyPressed(KEY_F)) {
          m_currentZone = ZoneType::CHAINED_CAVE;
          m_wizard.setPosition(m_cave.getEntrancePos());
          m_audioManager->playSound("portal_travel");
        }
        
        // Zone transition to Tree of Knowledge
        if (m_wizard.getPosition().x > 1800.0f && m_inputManager->isKeyPressed(KEY_F)) {
          m_currentZone = ZoneType::TREE_OF_KNOWLEDGE;
          m_wizard.setPosition(m_treeOfKnowledge.getEntrancePos());
          m_audioManager->playSound("portal_travel");
          m_questManager.onEnterTreeOfKnowledge(); 
        }
      } else if (m_currentZone == ZoneType::TREE_OF_KNOWLEDGE) {
        m_treeOfKnowledge.update(deltaTime, m_wizard, m_inputManager.get(), m_enemies,
                                 *m_audioManager, m_combatSystem.getParticles(), m_questManager);
                                 
        // Transition back to Sorting Arena
        if (m_wizard.getPosition().x < 150.0f && m_inputManager->isKeyPressed(KEY_F)) {
          m_currentZone = ZoneType::SORTING_ARENA;
          m_wizard.setPosition({ 1750.0f, m_wizard.getPosition().y });
          m_audioManager->playSound("portal_travel");
        }

        // Zone transition to Graph City
        if (m_wizard.getPosition().x > 1800.0f && m_inputManager->isKeyPressed(KEY_F)) {
          m_currentZone = ZoneType::GRAPH_CITY;
          m_wizard.setPosition(m_graphCity.getEntrancePos());
          m_audioManager->playSound("portal_travel");
          m_questManager.onEnterGraphCity();
        }
      } else if (m_currentZone == ZoneType::GRAPH_CITY) {
        m_graphCity.update(deltaTime, m_wizard, m_inputManager.get(), m_enemies,
                                 *m_audioManager, m_combatSystem.getParticles(), m_questManager);
                                 
        // Transition back to Tree of Knowledge
        if (m_wizard.getPosition().x < 150.0f && m_inputManager->isKeyPressed(KEY_F)) {
          m_currentZone = ZoneType::TREE_OF_KNOWLEDGE;
          m_wizard.setPosition({ 1750.0f, m_wizard.getPosition().y });
          m_audioManager->playSound("portal_travel");
        }
      }

      // 3. Update Audio Spell System (Musical keyboard notes, combos, Stack undo)
      m_combatSystem.setActiveEnemies(&m_enemies);
      m_spellSystem.update(deltaTime, *m_inputManager, m_wizard, m_world, 
                           m_combatSystem, *m_audioManager);

      // 4. Update Combat System (projectiles, floating numbers, particles)
      m_combatSystem.update(deltaTime, m_wizard, m_enemies, *m_audioManager);

      // 5. Update Enemy AI & Defeat Logic
      for (auto& enemy : m_enemies) {
        if (!enemy) continue;

        enemy->updateAI(deltaTime, m_wizard, m_world, *m_audioManager, m_combatSystem.getParticles());

        // Check if enemy defeated
        if (enemy->isDead()) {
          auto* creature = dynamic_cast<CorruptedCreature*>(enemy.get());
          if (creature && !creature->hasAwardedXP()) {
            creature->markXPAwarded();
            m_wizard.getStats().addExperience(creature->getXPReward());
            m_wizard.getStats().addDSAMastery(5.0f);
            m_combatSystem.addFloatingText(m_wizard.getPosition(), "+100 XP (Creature Slayed!)", GOLD, 18);
            m_questManager.onEnemyDefeated();
          }

          auto* bat = dynamic_cast<ShadowBat*>(enemy.get());
          if (bat && !bat->isDead()) {
            m_wizard.getStats().addExperience(40);
            m_wizard.getStats().addDSAMastery(3.0f);
            m_combatSystem.addFloatingText(m_wizard.getPosition(), "+40 XP", GOLD, 16);
          }

          auto* crawler = dynamic_cast<StoneCrawler*>(enemy.get());
          if (crawler && !crawler->isDead()) {
            m_wizard.getStats().addExperience(60);
            m_wizard.getStats().addDSAMastery(4.0f);
            m_combatSystem.addFloatingText(m_wizard.getPosition(), "+60 XP", GOLD, 16);
          }

          auto* guardian = dynamic_cast<ChainGuardian*>(enemy.get());
          if (guardian && !guardian->isDead()) {
            m_wizard.getStats().addExperience(300);
            m_wizard.getStats().addDSAMastery(25.0f);
            m_combatSystem.addFloatingText(m_wizard.getPosition(), "+300 XP (Guardian Defeated!)", GOLD, 20);
            m_questManager.onGuardianDefeated();
          }

          auto* serpent = dynamic_cast<SortingSerpent*>(enemy.get());
          if (serpent && !serpent->isDead()) {
            m_wizard.getStats().addExperience(450);
            m_wizard.getStats().addDSAMastery(35.0f);
            m_combatSystem.addFloatingText(m_wizard.getPosition(), "+450 XP (Sorting Serpent Vanquished!)", GOLD, 22);
            m_questManager.onGuardianDefeated();
          }

          auto* treeGuardian = dynamic_cast<TreeGuardian*>(enemy.get());
          if (treeGuardian && !treeGuardian->isDead()) {
            m_wizard.getStats().addExperience(500);
            m_wizard.getStats().addDSAMastery(40.0f);
            m_combatSystem.addFloatingText(m_wizard.getPosition(), "+500 XP (Tree Guardian Defeated!)", GOLD, 22);
            m_questManager.onTreeGuardianDefeated();
          }
        }
      }

      // 6. Update Quest
      m_questManager.update(deltaTime);

      // 7. Check Victory condition
      if (m_questManager.isQuestCompleted() && !m_victoryShown) {
        m_victoryShown = true;
        m_currentState = GameState::VICTORY;
      }

      // 8. Check Player Defeat
      if (m_wizard.getStats().getHealth() <= 0) {
        m_currentState = GameState::GAME_OVER;
      }

      // 9. Camera smooth follow
      m_camera.target.x +=
          (m_wizard.getPosition().x - m_camera.target.x) * 5.0f * deltaTime;
      m_camera.target.y +=
          (m_wizard.getPosition().y - m_camera.target.y) * 5.0f * deltaTime;

      // Clamp camera to active zone bounds
      float halfWidth = 800.0f / (2.0f * m_camera.zoom);
      float halfHeight = 600.0f / (2.0f * m_camera.zoom);

      if (m_camera.target.x < currentBounds.x + halfWidth)
        m_camera.target.x = currentBounds.x + halfWidth;
      if (m_camera.target.y < currentBounds.y + halfHeight)
        m_camera.target.y = currentBounds.y + halfHeight;
      if (m_camera.target.x > currentBounds.width - halfWidth)
        m_camera.target.x = currentBounds.width - halfWidth;
      if (m_camera.target.y > currentBounds.height - halfHeight)
        m_camera.target.y = currentBounds.height - halfHeight;
    }
    break;

  case GameState::PAUSE:
    if (m_inputManager->isKeyPressed(KEY_ESCAPE)) {
      m_currentState = GameState::EXPLORATION;
    }

    if (m_inputManager->isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePos = m_inputManager->getMousePosition();
      int sw = GetScreenWidth();
      int px = sw / 2 - 180;
      float startX = px + 40.0f;
      float endX = px + 360.0f - 40.0f; // 280px wide
      
      if (mousePos.x > startX && mousePos.x < endX) {
        if (mousePos.y > 160 && mousePos.y < 200) {
          // Resume
          m_currentState = GameState::EXPLORATION;
        } else if (mousePos.y > 270 && mousePos.y < 310) {
          // DSA Mastery
          m_masteryPanel.show();
          m_currentState = GameState::EXPLORATION;
        } else if (mousePos.y > 435 && mousePos.y < 475) {
          // Main Menu
          restartGame();
          m_currentState = GameState::MAIN_MENU;
        }
      }
    }
    break;

  case GameState::VICTORY:
    if (m_inputManager->isKeyPressed(KEY_ENTER) || 
        m_inputManager->isKeyPressed(KEY_ESCAPE) ||
        m_inputManager->isMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      m_currentState = GameState::EXPLORATION;
    }
    break;

  case GameState::GAME_OVER:
    if (m_inputManager->isKeyPressed(KEY_ENTER)) {
      restartGame();
      m_currentState = GameState::EXPLORATION;
    } else if (m_inputManager->isKeyPressed(KEY_ESCAPE)) {
      restartGame();
      m_currentState = GameState::MAIN_MENU;
    }
    break;
  }

  m_audioManager->update();
}

void Game::render() {
  BeginDrawing();
  ClearBackground(Color{10, 10, 15, 255});

  if (m_currentState == GameState::MAIN_MENU) {
    m_hud.drawMainMenu();
  } else {
    // Draw 2D World through Camera
    BeginMode2D(m_camera);
    
    if (m_currentZone == ZoneType::FOREST_OF_AWAKENING) {
      m_world.draw();
    } else if (m_currentZone == ZoneType::CHAINED_CAVE) {
      m_cave.draw();
    } else if (m_currentZone == ZoneType::SORTING_ARENA) {
      m_sortingArena.draw();
    } else if (m_currentZone == ZoneType::TREE_OF_KNOWLEDGE) {
      m_treeOfKnowledge.draw();
    } else if (m_currentZone == ZoneType::GRAPH_CITY) {
      m_graphCity.draw();
    }

    // Draw enemies
    for (const auto& enemy : m_enemies) {
      if (enemy) {
        enemy->draw();
      }
    }

    // Draw combat projectiles and particles in world space
    m_combatSystem.draw();

    // Draw Wizard
    m_wizard.draw();

    EndMode2D();

    // Draw Screen-space Overlay HUD
    const ChainedCave* cavePtr = (m_currentZone == ZoneType::CHAINED_CAVE) ? &m_cave : nullptr;
    m_hud.drawHUD(m_wizard, m_world, cavePtr, m_spellSystem, m_combatSystem, 
                  m_questManager, m_enemies, &m_masteryPanel);

    // State overlays
    if (m_currentState == GameState::PAUSE) {
      m_hud.drawPauseMenu();
    } else if (m_currentState == GameState::VICTORY) {
      m_hud.drawVictoryScreen();
    } else if (m_currentState == GameState::GAME_OVER) {
      m_hud.drawGameOverScreen();
    }
  }

  EndDrawing();
}

