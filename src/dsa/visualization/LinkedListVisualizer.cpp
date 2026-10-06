#include "LinkedListVisualizer.hpp"
#include <algorithm>

LinkedListVisualizer::LinkedListVisualizer() = default;

void LinkedListVisualizer::setMessage(const std::string& msg, float duration) {
    m_message = msg;
    m_messageTimer = duration;
}

void LinkedListVisualizer::update(float deltaTime) {
    if (m_messageTimer > 0.0f) {
        m_messageTimer -= deltaTime;
    }
}

void LinkedListVisualizer::draw(int x, int y, const std::vector<VisualNodeInfo>& nodes, bool isReversed) const {
    int panelW = 380;
    int panelH = 92;
    DrawRectangle(x, y, panelW, panelH, Color{15, 15, 25, 235});
    DrawRectangleLines(x, y, panelW, panelH, Color{70, 70, 100, 255});

    DrawText("CRYSTAL CHAIN [LINKED LIST]", x + 10, y + 8, 10, GOLD);

    if (isReversed) {
        DrawText("(Reversed)", x + 215, y + 8, 10, SKYBLUE);
    }

    int startX = x + 12;
    int nodeY = y + 26;
    int nodeW = 54;
    int nodeH = 26;
    int spacing = 26;

    // Draw HEAD pointer
    DrawText("HEAD", startX, nodeY + 6, 9, GOLD);
    startX += 36;
    DrawText(isReversed ? "<-" : "->", startX, nodeY + 6, 12, GRAY);
    startX += 22;

    for (size_t i = 0; i < nodes.size(); ++i) {
        // Node Box
        DrawRectangle(startX, nodeY, nodeW, nodeH, Color{25, 25, 40, 255});
        DrawRectangleLines(startX, nodeY, nodeW, nodeH, nodes[i].color);

        int textW = MeasureText(nodes[i].label.c_str(), 10);
        DrawText(nodes[i].label.c_str(), startX + nodeW / 2 - textW / 2, nodeY + 8, 10, nodes[i].color);

        startX += nodeW;

        // Pointer Arrow
        if (i + 1 < nodes.size()) {
            DrawText(isReversed ? "<-" : "->", startX + 6, nodeY + 6, 12, SKYBLUE);
            startX += spacing;
        }
    }

    // NULL pointer
    startX += 6;
    DrawText(isReversed ? "<-" : "->", startX, nodeY + 6, 12, GRAY);
    startX += 22;
    DrawText("NULL", startX, nodeY + 6, 9, GRAY);

    // Educational / Action message
    if (m_messageTimer > 0.0f) {
        DrawText(m_message.c_str(), x + 10, y + 66, 11, YELLOW);
    } else {
        DrawText("Nodes connect with pointers. Press [Z] Insert, [X] Remove, [C] Reverse", 
                 x + 10, y + 66, 9, LIGHTGRAY);
    }
}
