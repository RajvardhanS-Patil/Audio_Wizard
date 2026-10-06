#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>
#include "../player/Wizard.hpp"
#include "../entities/Enemy.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include "../quests/QuestManager.hpp"
#include "../dsa/structures/BinarySearchTree.hpp"
#include "../dsa/visualization/TreeVisualizer.hpp"
#include "../entities/TreeGuardian.hpp"

class TreeOfKnowledge {
public:
    TreeOfKnowledge();

    void update(float deltaTime, Wizard& wizard, const class InputManager* inputManager,
                std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                ParticleSystem& particles, QuestManager& questManager);

    void draw() const;

    Rectangle getBounds() const { return m_bounds; }
    Vector2 getEntrancePos() const { return { 200.0f, 300.0f }; }

private:
    void initEnvironment();
    void updateTreeLogic(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                         AudioManager& audioManager, QuestManager& questManager);

    Rectangle m_bounds;
    
    // Environment
    struct Decor {
        Vector2 position;
        float radius;
        Color color;
    };
    std::vector<Decor> m_decorations;

    // Tree Logic
    BinarySearchTree<int> m_bst;
    TreeVisualizer m_visualizer;

    float m_stateTimer{0.0f};
    bool m_guardianSpawned{false};
    bool m_treeRestored{false};
    
    // UI Interaction
    bool m_showInteraction{false};
    std::string m_interactionMessage;
    int m_highlightedNode{-1};
    
    // Traversal Challenge
    std::vector<int> m_expectedTraversal;
    int m_traversalIndex{0};
    bool m_inTraversalChallenge{false};
};
