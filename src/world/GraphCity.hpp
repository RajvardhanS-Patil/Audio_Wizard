#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../quests/QuestManager.hpp"
#include "../dsa/structures/Graph.hpp"

enum class NodeState {
    CORRUPTED,
    RESTORED
};

struct CityNode {
    std::string id;
    std::string name;
    Vector2 position;
    NodeState state;
    bool discovered;
};

class GraphCity {
public:
    GraphCity();
    
    void update(float deltaTime, Wizard& wizard, const class InputManager* inputManager,
                std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                ParticleSystem& particles, QuestManager& questManager);
                
    void draw() const;
    
    Rectangle getBounds() const { return m_bounds; }
    Vector2 getEntrancePos() const { return { 200.0f, 600.0f }; }

private:
    void initCityGraph();
    void updateGraphLogic(float deltaTime, Wizard& wizard, const class InputManager* inputManager,
                          AudioManager& audioManager, QuestManager& questManager);
    void drawNodes() const;
    void drawEdges() const;

    Rectangle m_bounds;
    
    // The core Graph structure
    Graph<std::string> m_cityGraph;
    
    // Metadata for nodes
    std::unordered_map<std::string, CityNode> m_cityNodes;
    
    bool m_showInteraction{false};
    std::string m_interactionMessage;
    
    std::string m_currentNodeId;
};
