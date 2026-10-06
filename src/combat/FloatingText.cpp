#include "FloatingText.hpp"

void FloatingTextManager::add(Vector2 position, const std::string& text, Color color, int fontSize, float duration) {
    FloatingTextItem item;
    item.position = position;
    item.text = text;
    item.color = color;
    item.lifetime = duration;
    item.maxLifetime = duration;
    item.fontSize = fontSize;
    m_items.push_back(item);
}

void FloatingTextManager::update(float deltaTime) {
    for (auto it = m_items.begin(); it != m_items.end();) {
        it->lifetime -= deltaTime;
        if (it->lifetime <= 0.0f) {
            it = m_items.erase(it);
        } else {
            // Drift upward
            it->position.y -= 35.0f * deltaTime;
            ++it;
        }
    }
}

void FloatingTextManager::draw() const {
    for (const auto& item : m_items) {
        float alphaRatio = (item.maxLifetime > 0.0f) ? (item.lifetime / item.maxLifetime) : 0.0f;
        Color col = item.color;
        col.a = static_cast<unsigned char>(item.color.a * alphaRatio);

        int textWidth = MeasureText(item.text.c_str(), item.fontSize);
        DrawText(item.text.c_str(), 
                 static_cast<int>(item.position.x - textWidth / 2), 
                 static_cast<int>(item.position.y), 
                 item.fontSize, col);
    }
}

void FloatingTextManager::clear() {
    m_items.clear();
}
