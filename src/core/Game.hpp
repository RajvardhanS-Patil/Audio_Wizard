#pragma once

#include "../audio/AudioManager.hpp"
#include "../player/Wizard.hpp"
#include "../ui/HUD.hpp"
#include "../ui/DSAMasteryPanel.hpp"
#include "../world/World.hpp"
#include "../world/ChainedCave.hpp"
#include "../world/SortingArena.hpp"
#include "../world/TreeOfKnowledge.hpp"
#include "../world/GraphCity.hpp"
#include "../spells/AudioSpellSystem.hpp"
#include "../combat/CombatSystem.hpp"
#include "../quests/QuestManager.hpp"
#include "../entities/Enemy.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include <memory>
#include <vector>
#include <raylib.h>

enum class ZoneType {
    FOREST_OF_AWAKENING,
    CHAINED_CAVE,
    SORTING_ARENA,
    TREE_OF_KNOWLEDGE,
    GRAPH_CITY
};

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void restartGame();

    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<AudioManager> m_audioManager;

    GameState m_currentState{GameState::MAIN_MENU};
    ZoneType m_currentZone{ZoneType::FOREST_OF_AWAKENING};

    Wizard m_wizard;
    World m_world;
    ChainedCave m_cave;
    SortingArena m_sortingArena;
    TreeOfKnowledge m_treeOfKnowledge;
    GraphCity m_graphCity;
    HUD m_hud;
    DSAMasteryPanel m_masteryPanel;

    AudioSpellSystem m_spellSystem;
    CombatSystem m_combatSystem;
    QuestManager m_questManager;
    std::vector<std::unique_ptr<Enemy>> m_enemies;

    Camera2D m_camera = {0};
    bool m_victoryShown{false};
};

