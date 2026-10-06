#include "QueueVisualizer.hpp"

QueueVisualizer::QueueVisualizer() = default;

void QueueVisualizer::setMessage(const std::string& msg, float duration) {
    m_message = msg;
    m_messageTimer = duration;
}

void QueueVisualizer::update(float deltaTime) {
    if (m_messageTimer > 0.0f) {
        m_messageTimer -= deltaTime;
    }
}

void QueueVisualizer::draw(int x, int y, const std::vector<VisualQueueItem>& items) const {
    int panelW = 320;
    int panelH = 75;
    DrawRectangle(x, y, panelW, panelH, Color{15, 15, 25, 235});
    DrawRectangleLines(x, y, panelW, panelH, Color{70, 70, 100, 255});

    DrawText("ECHO GATE [QUEUE - FIFO]", x + 10, y + 8, 10, GOLD);

    if (items.empty()) {
        DrawText("No enemies waiting in queue. Gate Clear!", x + 12, y + 30, 11, LIME);
        return;
    }

    int startX = x + 12;
    int itemY = y + 26;
    int itemW = 58;
    int itemH = 22;

    DrawText("FRONT", startX, itemY + 4, 8, SKYBLUE);
    startX += 36;

    for (size_t i = 0; i < items.size(); ++i) {
        DrawRectangle(startX, itemY, itemW, itemH, Color{25, 25, 40, 255});
        DrawRectangleLines(startX, itemY, itemW, itemH, items[i].color);

        int tw = MeasureText(items[i].label.c_str(), 9);
        DrawText(items[i].label.c_str(), startX + itemW / 2 - tw / 2, itemY + 6, 9, items[i].color);

        startX += itemW + 4;
        if (i + 1 < items.size()) {
            DrawText("<-", startX - 2, itemY + 4, 10, GRAY);
            startX += 10;
        }
    }

    DrawText("REAR", startX, itemY + 4, 8, ORANGE);

    if (m_messageTimer > 0.0f) {
        DrawText(m_message.c_str(), x + 10, y + 54, 10, YELLOW);
    } else {
        DrawText("First in, first out. Enemies enter in order.", x + 10, y + 54, 9, LIGHTGRAY);
    }
}
