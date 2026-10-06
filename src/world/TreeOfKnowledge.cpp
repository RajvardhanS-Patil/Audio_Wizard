#include "TreeOfKnowledge.hpp"
#include "../core/InputManager.hpp"
#include <raymath.h>

TreeOfKnowledge::TreeOfKnowledge() {
    m_bounds = { 0.0f, 0.0f, 2400.0f, 1800.0f };
    initEnvironment();

    // Setup initial BST for the Tree of Knowledge
    m_bst.insert(8);
    m_bst.insert(4);
    m_bst.insert(12);
    m_bst.insert(2);
    m_bst.insert(6);
}

void TreeOfKnowledge::initEnvironment() {
    // Add some ancient forest decor
    for (int i = 0; i < 40; ++i) {
        Decor d;
        d.position = { (float)GetRandomValue(50, 2350), (float)GetRandomValue(50, 1750) };
        d.radius = (float)GetRandomValue(10, 40);
        d.color = ColorAlpha(DARKGREEN, GetRandomValue(40, 100) / 255.0f);
        m_decorations.push_back(d);
    }
}

void TreeOfKnowledge::update(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                             std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                             ParticleSystem& particles, QuestManager& questManager) {
    m_visualizer.update(deltaTime);

    updateTreeLogic(deltaTime, wizard, inputManager, audioManager, questManager);

    // Placeholder for Guardian Spawn Logic
    if (!m_guardianSpawned && m_treeRestored) {
        // Spawn Tree Guardian
        enemies.push_back(std::make_unique<TreeGuardian>(Vector2{1800.0f, 600.0f}));
        m_guardianSpawned = true;
    }
}

void TreeOfKnowledge::updateTreeLogic(float deltaTime, Wizard& wizard, const InputManager* inputManager,
                                      AudioManager& audioManager, QuestManager& questManager) {
    Vector2 wPos = wizard.getPosition();
    Vector2 treeCenter = { 1200.0f, 400.0f };

    m_showInteraction = false;
    
    // Near Tree
    if (Vector2Distance(wPos, treeCenter) < 400.0f) {
        m_showInteraction = true;
        if (!m_inTraversalChallenge) {
            m_interactionMessage = "Press [E] to begin Inorder Traversal Challenge";
            if (inputManager->isKeyPressed(KEY_E)) {
                m_inTraversalChallenge = true;
                m_expectedTraversal = m_bst.inorder();
                m_traversalIndex = 0;
                m_highlightedNode = -1;
                audioManager.playSound("spell_cast"); // Replace with magic sound
            }
        } else {
            m_interactionMessage = "Inorder: Left -> Root -> Right. Current target: " + std::to_string(m_expectedTraversal[m_traversalIndex]);
            
            // Simple interaction: press corresponding number key (hacky for prototype)
            // or just press E to simulate stepping through. Let's step through with E.
            if (inputManager->isKeyPressed(KEY_E)) {
                m_highlightedNode = m_expectedTraversal[m_traversalIndex];
                m_traversalIndex++;
                audioManager.playSound("crystal_activate");
                
                if (m_traversalIndex >= m_expectedTraversal.size()) {
                    m_inTraversalChallenge = false;
                    m_treeRestored = true;
                    wizard.getStats().addDSAMastery(20.0f); // Reward mastery
                    audioManager.playSound("level_up");
                    m_interactionMessage = "Tree Restored! Knowledge Mastery increased.";
                    m_showInteraction = true;
                }
            }
        }
    }
}

void TreeOfKnowledge::draw() const {
    // Draw background
    DrawRectangleRec(m_bounds, Color{15, 20, 20, 255}); // Dark forest theme

    // Draw decor
    for (const auto& d : m_decorations) {
        DrawCircleV(d.position, d.radius, d.color);
    }

    // Draw Ancient Tree base
    Vector2 treeBase = { 1200.0f, 800.0f };
    DrawCircleV(treeBase, 150.0f, Fade(BROWN, 0.4f));
    DrawText("Tree of Knowledge", treeBase.x - 100, treeBase.y - 180, 24, GOLD);

    // Draw BST visualizer
    m_visualizer.draw(m_bst.getRoot(), { 1200.0f, 300.0f }, 200.0f, 100.0f, m_highlightedNode);

    // Draw UI Interaction
    if (m_showInteraction) {
        int textWidth = MeasureText(m_interactionMessage.c_str(), 20);
        DrawRectangle(1200 - textWidth/2 - 10, 850, textWidth + 20, 30, Fade(BLACK, 0.7f));
        DrawText(m_interactionMessage.c_str(), 1200 - textWidth/2, 855, 20, WHITE);
    }
}
