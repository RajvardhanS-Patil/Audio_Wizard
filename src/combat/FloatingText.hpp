#pragma once

#include <raylib.h>
#include <string>
#include <vector>

struct FloatingTextItem {
    Vector2 position{0.0f, 0.0f};
    std::string text;
    Color color{WHITE};
    float lifetime{1.0f};
    float maxLifetime{1.0f};
    int fontSize{16};
};

class FloatingTextManager {
public:
    FloatingTextManager() = default;

    void add(Vector2 position, const std::string& text, Color color, int fontSize = 16, float duration = 1.0f);
    void update(float deltaTime);
    void draw() const;
    void clear();

private:
    std::vector<FloatingTextItem> m_items;
};
