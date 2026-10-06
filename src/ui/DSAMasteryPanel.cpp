#include "DSAMasteryPanel.hpp"
#include "../core/InputManager.hpp"
#include <raylib.h>

DSAMasteryPanel::DSAMasteryPanel() {
    m_topics = {
        {"Array", 100.0f, "Contiguous memory allocation.", "Movement history.", "N/A"},
        {"Linked List", 100.0f, "Node-based linear structure.", "Chain Link Spell.", "Chain Link"},
        {"Queue", 100.0f, "FIFO structure.", "Queue Surge Spell.", "Queue Surge"},
        {"Stack", 100.0f, "LIFO structure.", "Spell Undo feature.", "Undo Visualizer"},
        {"Sorting", 100.0f, "Algorithmic sorting techniques.", "Sorting spells (Bubble/Merge/Quick).", "Bubble Wave, Merge Magic, Quick Strike"},
        {"Tree", 100.0f, "Hierarchical structure with a root and children.", "Traversals and hierarchical paths.", "Root Bind"},
        {"Binary Tree", 100.0f, "A tree where nodes have at most two children.", "Level-based structural discovery.", "Tree Vision"},
        {"Binary Search Tree", 100.0f, "A sorted binary tree.", "O(log n) efficient search and insertion.", "Growth Magic"},
        {"Tree Traversals", 100.0f, "Inorder, Preorder, Postorder, Level-Order.", "Traversing ancient paths.", "Traversal Mastery"},
        {"Graph", 100.0f, "A network of interconnected nodes (vertices) and lines (edges).", "Graph City Navigation.", "Node Discovery"},
        {"Vertex & Edge", 100.0f, "A vertex is a point; an edge is a connection between points.", "Restoring locations and magical roads.", "City Restoration"},
        {"Adjacency List", 100.0f, "Stores a list of neighbors for each vertex.", "Underlying data structure of the city map.", "Road Network"},
        {"BFS & DFS", 100.0f, "Breadth-First and Depth-First Search for exploring graphs.", "Finding shortest paths or deep connections.", "Pathfinder"}
    };
}

void DSAMasteryPanel::update(float deltaTime, const InputManager* inputManager) {
    if (!m_visible || !inputManager) return;
    
    if (inputManager->isKeyPressed(KEY_UP)) {
        m_selectedIndex--;
        if (m_selectedIndex < 0) m_selectedIndex = (int)m_topics.size() - 1;
    }
    if (inputManager->isKeyPressed(KEY_DOWN)) {
        m_selectedIndex++;
        if (m_selectedIndex >= (int)m_topics.size()) m_selectedIndex = 0;
    }
    
    if (inputManager->isKeyPressed(KEY_ESCAPE) || inputManager->isKeyPressed(KEY_M) || inputManager->isKeyPressed(KEY_TAB)) {
        m_visible = false;
    }
}

void DSAMasteryPanel::draw() const {
    if (!m_visible) return;
    
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    int panelW = 700;
    int panelH = 460;
    int panelX = (sw - panelW) / 2;
    int panelY = (sh - panelH) / 2;
    
    // Background and magical border
    DrawRectangleRounded({(float)panelX, (float)panelY, (float)panelW, (float)panelH}, 0.1f, 8, Color{15, 20, 35, 240});
    DrawRectangleRoundedLinesEx({(float)panelX, (float)panelY, (float)panelW, (float)panelH}, 0.1f, 8, 2.0f, GOLD);
    
    // Header
    DrawCircleGradient({(float)(panelX + panelW/2), (float)(panelY + 30)}, 150.0f, Color{50, 40, 10, 150}, BLANK);
    DrawText("DSA KNOWLEDGE", panelX + panelW/2 - MeasureText("DSA KNOWLEDGE", 28)/2, panelY + 20, 28, GOLD);
    DrawLine(panelX + 40, panelY + 60, panelX + panelW - 40, panelY + 60, Color{255, 215, 0, 100});
    
    // Left side: topics list background
    DrawRectangleRounded({(float)panelX + 20, (float)panelY + 70, 200.0f, (float)panelH - 120.0f}, 0.1f, 4, Color{10, 15, 25, 200});
    
    // Draw topics list (with a scroll offset calculation so it doesn't overflow)
    int maxVisible = 11;
    int scrollOffset = 0;
    if (m_selectedIndex >= maxVisible) {
        scrollOffset = m_selectedIndex - maxVisible + 1;
    }
    
    for (size_t i = scrollOffset; i < m_topics.size() && (i - scrollOffset) < (size_t)maxVisible; ++i) {
        int itemIndex = i - scrollOffset;
        int itemY = panelY + 80 + (int)(itemIndex * 30); // Reduced spacing
        if ((int)i == m_selectedIndex) {
            DrawRectangleRounded({(float)panelX + 20, (float)itemY - 5, 200.0f, 25.0f}, 0.1f, 4, Color{40, 60, 100, 200});
            DrawText(m_topics[i].name.c_str(), panelX + 35, itemY, 18, GOLD);
        } else {
            DrawText(m_topics[i].name.c_str(), panelX + 35, itemY, 16, LIGHTGRAY);
        }
    }
    
    // Draw details for selected
    if (m_selectedIndex >= 0 && m_selectedIndex < (int)m_topics.size()) {
        const auto& t = m_topics[m_selectedIndex];
        int dx = panelX + 250;
        int dy = panelY + 70;
        
        // Topic Title
        DrawText(t.name.c_str(), dx, dy, 24, WHITE);
        
        // Status & Progress bar
        DrawText("Status: MASTERED", dx, dy + 35, 14, GREEN);
        DrawRectangleRounded({(float)dx, (float)dy + 55, 300.0f, 12.0f}, 0.5f, 4, DARKGRAY);
        DrawRectangleRounded({(float)dx, (float)dy + 55, 300.0f * (t.percentage / 100.0f), 12.0f}, 0.5f, 4, GREEN);
        
        // Info Box
        DrawRectangleRounded({(float)dx, (float)dy + 85, 410.0f, 220.0f}, 0.1f, 4, Color{10, 15, 25, 180});
        
        DrawText("Concept:", dx + 15, dy + 100, 16, GOLD);
        DrawText(t.summary.c_str(), dx + 15, dy + 120, 16, LIGHTGRAY);
        
        DrawText("Gameplay Application:", dx + 15, dy + 160, 16, SKYBLUE);
        DrawText(t.gameplayUsage.c_str(), dx + 15, dy + 180, 16, LIGHTGRAY);
        
        DrawText("Arcane Manifestation (Unlocks):", dx + 15, dy + 220, 16, MAGENTA);
        DrawText(t.unlockedAbility.c_str(), dx + 15, dy + 240, 16, WHITE);
    }
    
    DrawLine(panelX + 40, panelY + panelH - 50, panelX + panelW - 40, panelY + panelH - 50, Color{255, 215, 0, 50});
    const char* footer = "Press [UP/DOWN] to Navigate   |   Press [ESC/M/TAB] to Close";
    DrawText(footer, panelX + panelW/2 - MeasureText(footer, 16)/2, panelY + panelH - 35, 16, GRAY);
}
