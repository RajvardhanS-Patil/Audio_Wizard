#include "GraphCity.hpp"
#include "../core/InputManager.hpp"
#include <raymath.h>

GraphCity::GraphCity() : m_cityGraph(false) { // Undirected graph
    m_bounds = { 0.0f, 0.0f, 2400.0f, 1800.0f };
    initCityGraph();
}

void GraphCity::initCityGraph() {
    // Initialize nodes metadata
    m_cityNodes["Castle"] = {"Castle", "Ancient Castle", {1200.0f, 300.0f}, NodeState::CORRUPTED, false};
    m_cityNodes["Forest"] = {"Forest", "Whispering Forest", {800.0f, 600.0f}, NodeState::CORRUPTED, false};
    m_cityNodes["Library"] = {"Library", "Grand Library", {1600.0f, 600.0f}, NodeState::CORRUPTED, false};
    m_cityNodes["Cave"] = {"Cave", "Crystal Cave", {600.0f, 1000.0f}, NodeState::CORRUPTED, false};
    m_cityNodes["Market"] = {"Market", "Bustling Market", {1200.0f, 1000.0f}, NodeState::CORRUPTED, false};
    m_cityNodes["Tower"] = {"Tower", "Arcane Tower", {1200.0f, 1500.0f}, NodeState::CORRUPTED, false};

    // Construct the graph
    m_cityGraph.addVertex("Castle");
    m_cityGraph.addVertex("Forest");
    m_cityGraph.addVertex("Library");
    m_cityGraph.addVertex("Cave");
    m_cityGraph.addVertex("Market");
    m_cityGraph.addVertex("Tower");

    // Add edges (magical roads)
    m_cityGraph.addEdge("Castle", "Forest");
    m_cityGraph.addEdge("Castle", "Library");
    m_cityGraph.addEdge("Forest", "Cave");
    m_cityGraph.addEdge("Forest", "Market");
    m_cityGraph.addEdge("Library", "Market");
    m_cityGraph.addEdge("Cave", "Tower");
    m_cityGraph.addEdge("Market", "Tower");
}

void GraphCity::update(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                       std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                       ParticleSystem& particles, QuestManager& questManager) {
    
    updateGraphLogic(deltaTime, wizard, inputManager, audioManager, questManager);
}

void GraphCity::updateGraphLogic(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                                 AudioManager& audioManager, QuestManager& questManager) {
    Vector2 wPos = wizard.getPosition();
    m_showInteraction = false;
    m_currentNodeId = "";

    // Check distance to all nodes
    for (auto& pair : m_cityNodes) {
        CityNode& node = pair.second;
        if (Vector2Distance(wPos, node.position) < 150.0f) {
            m_showInteraction = true;
            m_currentNodeId = node.id;
            
            if (!node.discovered) {
                node.discovered = true;
                audioManager.playSound("spell_cast"); // Discover sound
            }

            if (node.state == NodeState::CORRUPTED) {
                m_interactionMessage = "Press [E] to Restore " + node.name;
                
                if (inputManager->isKeyPressed(KEY_E)) {
                    node.state = NodeState::RESTORED;
                    audioManager.playSound("crystal_activate");
                    wizard.getStats().addDSAMastery(10.0f);
                    
                    // Check if all nodes are restored
                    bool allRestored = true;
                    for (const auto& n : m_cityNodes) {
                        if (n.second.state != NodeState::RESTORED) {
                            allRestored = false;
                            break;
                        }
                    }
                    if (allRestored) {
                        questManager.onGraphCityRestored();
                    }
                }
            } else {
                m_interactionMessage = node.name + " (Restored Vertex)";
            }
            break; // Only interact with one node at a time
        }
    }
}

void GraphCity::draw() const {
    // Draw background
    DrawRectangleRec(m_bounds, Color{20, 20, 30, 255}); // Dark city theme

    // Draw Edges (Magical Roads)
    drawEdges();

    // Draw Nodes (Locations)
    drawNodes();

    // Draw UI Interaction
    if (m_showInteraction && !m_interactionMessage.empty()) {
        int textWidth = MeasureText(m_interactionMessage.c_str(), 20);
        DrawRectangle(1200 - textWidth/2 - 10, 850, textWidth + 20, 30, Fade(BLACK, 0.7f)); // Fixed at bottom of screen visually, or stick to player
        
        // Actually it's better to render interaction near the node or bottom screen
        // In Game.cpp camera follows player, so fixed coords won't work perfectly unless relative to camera.
        // Let's use the node position.
        if (m_cityNodes.find(m_currentNodeId) != m_cityNodes.end()) {
            Vector2 pos = m_cityNodes.at(m_currentNodeId).position;
            DrawRectangle(pos.x - textWidth/2 - 10, pos.y + 80, textWidth + 20, 30, Fade(BLACK, 0.7f));
            DrawText(m_interactionMessage.c_str(), pos.x - textWidth/2, pos.y + 85, 20, WHITE);
        }
    }
}

void GraphCity::drawEdges() const {
    // We iterate over the adjacency list to draw edges
    auto adjList = m_cityGraph.getAdjacencyList();
    for (const auto& pair : adjList) {
        const std::string& u = pair.first;
        for (const auto& edge : pair.second) {
            const std::string& v = edge.first;
            
            if (m_cityNodes.find(u) != m_cityNodes.end() && m_cityNodes.find(v) != m_cityNodes.end()) {
                const CityNode& nodeU = m_cityNodes.at(u);
                const CityNode& nodeV = m_cityNodes.at(v);
                
                Color roadColor = (nodeU.state == NodeState::RESTORED && nodeV.state == NodeState::RESTORED) 
                                  ? Color{0, 228, 255, 150} // Restored cyan glow
                                  : Color{130, 20, 40, 150}; // Corrupted red glow
                
                DrawLineEx(nodeU.position, nodeV.position, 10.0f, roadColor);
            }
        }
    }
}

void GraphCity::drawNodes() const {
    for (const auto& pair : m_cityNodes) {
        const CityNode& node = pair.second;
        
        Color nodeColor = (node.state == NodeState::RESTORED) ? SKYBLUE : DARKPURPLE;
        DrawCircleV(node.position, 60.0f, nodeColor);
        DrawCircleLines(node.position.x, node.position.y, 60.0f, WHITE);
        
        if (node.discovered) {
            int textWidth = MeasureText(node.name.c_str(), 20);
            DrawText(node.name.c_str(), node.position.x - textWidth/2, node.position.y - 10, 20, WHITE);
            if (node.state == NodeState::RESTORED) {
                DrawText("(Vertex)", node.position.x - MeasureText("(Vertex)", 14)/2, node.position.y + 15, 14, GOLD);
            }
        } else {
            DrawText("???", node.position.x - MeasureText("???", 20)/2, node.position.y - 10, 20, GRAY);
        }
    }
}
