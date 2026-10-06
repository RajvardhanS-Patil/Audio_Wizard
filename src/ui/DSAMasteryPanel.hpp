#pragma once

#include <raylib.h>
#include <vector>
#include <string>

struct MasteryTopic {
    std::string name;
    float percentage;
    std::string summary;
    std::string gameplayUsage;
    std::string unlockedAbility;
};

class DSAMasteryPanel {
public:
    DSAMasteryPanel();

    void update(float deltaTime, const class InputManager* inputManager);
    void draw() const;

    void toggle() { m_visible = !m_visible; }
    void show() { m_visible = true; }
    void hide() { m_visible = false; }
    bool isVisible() const { return m_visible; }

private:
    bool m_visible{false};
    int m_selectedIndex{1}; // Default to Linked List for Phase 4
    std::vector<MasteryTopic> m_topics;
};
