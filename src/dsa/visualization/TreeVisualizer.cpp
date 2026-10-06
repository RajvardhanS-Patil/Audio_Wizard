#include "TreeVisualizer.hpp"
#include <raymath.h>

TreeVisualizer::TreeVisualizer() {}

void TreeVisualizer::update(float deltaTime) {
    m_pulseTime += deltaTime * 3.0f;
}

template <typename NodeType>
void TreeVisualizer::drawBranches(const NodeType* node, Vector2 position, float hOffset, float vOffset) const {
    if (!node) return;

    if (node->left) {
        Vector2 leftPos = { position.x - hOffset, position.y + vOffset };
        DrawLineEx(position, leftPos, 3.0f, Fade(SKYBLUE, 0.6f));
        drawBranches(node->left.get(), leftPos, hOffset * 0.5f, vOffset);
    }
    
    if (node->right) {
        Vector2 rightPos = { position.x + hOffset, position.y + vOffset };
        DrawLineEx(position, rightPos, 3.0f, Fade(SKYBLUE, 0.6f));
        drawBranches(node->right.get(), rightPos, hOffset * 0.5f, vOffset);
    }
}

template <typename NodeType>
void TreeVisualizer::drawRecursive(const NodeType* node, Vector2 position, float hOffset, float vOffset, int highlightedValue) const {
    if (!node) return;

    // First draw children
    if (node->left) {
        drawRecursive(node->left.get(), { position.x - hOffset, position.y + vOffset }, hOffset * 0.5f, vOffset, highlightedValue);
    }
    if (node->right) {
        drawRecursive(node->right.get(), { position.x + hOffset, position.y + vOffset }, hOffset * 0.5f, vOffset, highlightedValue);
    }

    // Determine visual style based on state
    bool isHighlighted = (node->value == highlightedValue);
    bool isTarget = (node->value == m_targetValue);

    float radius = 20.0f;
    Color nodeColor = DARKBLUE;
    Color glowColor = SKYBLUE;
    
    if (isTarget) {
        nodeColor = GOLD;
        glowColor = YELLOW;
        radius += sinf(m_pulseTime) * 3.0f;
    } else if (isHighlighted) {
        nodeColor = GREEN;
        glowColor = LIME;
        radius += 2.0f;
    }

    // Draw glowing aura
    DrawCircleV(position, radius + 4.0f, Fade(glowColor, 0.4f + 0.2f * sinf(m_pulseTime)));
    
    // Draw node body
    DrawCircleV(position, radius, nodeColor);
    DrawCircleLines(position.x, position.y, radius, WHITE);

    // Draw value
    std::string text = std::to_string(node->value);
    int textWidth = MeasureText(text.c_str(), 16);
    DrawText(text.c_str(), position.x - textWidth / 2, position.y - 8, 16, WHITE);
}

void TreeVisualizer::draw(const TreeNode<int>* root, Vector2 position, float nodeSpacingX, float nodeSpacingY, int highlightedValue) const {
    if (!root) return;
    drawBranches(root, position, nodeSpacingX, nodeSpacingY);
    drawRecursive(root, position, nodeSpacingX, nodeSpacingY, highlightedValue);
}

void TreeVisualizer::draw(const BTNode<int>* root, Vector2 position, float nodeSpacingX, float nodeSpacingY, int highlightedValue) const {
    if (!root) return;
    drawBranches(root, position, nodeSpacingX, nodeSpacingY);
    drawRecursive(root, position, nodeSpacingX, nodeSpacingY, highlightedValue);
}
