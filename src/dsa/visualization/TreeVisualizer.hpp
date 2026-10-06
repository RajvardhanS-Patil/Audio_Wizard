#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include "../structures/BinarySearchTree.hpp"
#include "../structures/BinaryTree.hpp"

class TreeVisualizer {
public:
    TreeVisualizer();
    
    void update(float deltaTime);
    
    // Draw the whole tree starting from the root
    void draw(const TreeNode<int>* root, Vector2 position, float nodeSpacingX, float nodeSpacingY, int highlightedValue = -1) const;
    void draw(const BTNode<int>* root, Vector2 position, float nodeSpacingX, float nodeSpacingY, int highlightedValue = -1) const;

    void setTargetValue(int val) { m_targetValue = val; }
    void clearTarget() { m_targetValue = -1; }
    
private:
    template <typename NodeType>
    void drawRecursive(const NodeType* node, Vector2 position, float hOffset, float vOffset, int highlightedValue) const;

    template <typename NodeType>
    void drawBranches(const NodeType* node, Vector2 position, float hOffset, float vOffset) const;

    float m_pulseTime{0.0f};
    int m_targetValue{-1};
};
