#pragma once

#include <raylib.h>
#include <vector>
#include <string>

struct VisualQueueItem {
    std::string label;
    Color color{WHITE};
};

class QueueVisualizer {
public:
    QueueVisualizer();

    void draw(int x, int y, const std::vector<VisualQueueItem>& items) const;
    void setMessage(const std::string& msg, float duration = 3.5f);
    void update(float deltaTime);

    bool hasMessage() const { return m_messageTimer > 0.0f; }

private:
    std::string m_message{""};
    float m_messageTimer{0.0f};
};
