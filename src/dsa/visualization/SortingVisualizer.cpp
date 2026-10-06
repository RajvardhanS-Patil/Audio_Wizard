#include "SortingVisualizer.hpp"
#include <raymath.h>

SortingVisualizer::SortingVisualizer() = default;

void SortingVisualizer::setArray(const DynamicArray<int>& array, const SortingAlgorithm& algorithm) {
    m_algorithmName = algorithm.getName();
    m_steps = algorithm.generateSteps(array);
    m_currentStepIndex = 0;
    m_stepTimer = 0.0f;
    m_active = false;
    m_finished = false;
    if (!m_steps.empty()) {
        m_currentDescription = m_steps[0].description;
    } else {
        m_currentDescription = "Ready to sort.";
    }
}

void SortingVisualizer::startVisualization() {
    m_active = true;
    m_finished = false;
    m_currentStepIndex = 0;
    m_stepTimer = 0.0f;
}

void SortingVisualizer::reset() {
    m_active = false;
    m_finished = false;
    m_currentStepIndex = 0;
    m_stepTimer = 0.0f;
}

void SortingVisualizer::cycleSpeed() {
    switch (m_speed) {
        case VisualizerSpeed::SLOW: m_speed = VisualizerSpeed::NORMAL; break;
        case VisualizerSpeed::NORMAL: m_speed = VisualizerSpeed::FAST; break;
        case VisualizerSpeed::FAST: m_speed = VisualizerSpeed::SLOW; break;
    }
}

const char* SortingVisualizer::getSpeedName() const {
    switch (m_speed) {
        case VisualizerSpeed::SLOW: return "Slow";
        case VisualizerSpeed::NORMAL: return "Normal";
        case VisualizerSpeed::FAST: return "Fast";
    }
    return "Normal";
}

void SortingVisualizer::update(float deltaTime) {
    if (!m_active || m_finished || m_steps.empty()) return;

    m_stepTimer += deltaTime;
    if (m_stepTimer >= getStepDuration()) {
        m_stepTimer = 0.0f;
        if (m_currentStepIndex + 1 < m_steps.size()) {
            m_currentStepIndex++;
            m_currentDescription = m_steps[m_currentStepIndex].description;
        } else {
            m_finished = true;
            m_active = false;
        }
    }
}

void SortingVisualizer::draw(int screenX, int screenY, int width, int height) const {
    // Draw visualizer container box
    DrawRectangle(screenX, screenY, width, height, ColorAlpha(DARKBLUE, 0.85f));
    DrawRectangleLines(screenX, screenY, width, height, GOLD);

    // Title & Speed Info
    DrawText(TextFormat("SORTING VISUALIZER: %s", m_algorithmName.c_str()), screenX + 15, screenY + 12, 18, GOLD);
    DrawText(TextFormat("Speed: %s [Tab]", getSpeedName()), screenX + width - 150, screenY + 12, 16, SKYBLUE);

    if (m_steps.empty()) {
        DrawText("No array loaded.", screenX + 20, screenY + 60, 16, LIGHTGRAY);
        return;
    }

    const SortingStep& currentStep = m_steps[m_currentStepIndex];
    const auto& arr = currentStep.snapshot;
    size_t count = arr.size();
    if (count == 0) return;

    int padding = 15;
    int availableWidth = width - (padding * 2);
    int elementWidth = (availableWidth / static_cast<int>(count)) - 8;
    if (elementWidth < 30) elementWidth = 30;

    int startX = screenX + padding + (availableWidth - (elementWidth + 8) * static_cast<int>(count)) / 2;
    int startY = screenY + height - 70;
    int maxBarHeight = height - 130;

    int maxVal = 1;
    for (int val : arr) {
        if (val > maxVal) maxVal = val;
    }

    // Operation Label
    const char* opTag = "SORTING";
    Color opColor = RAYWHITE;
    switch (currentStep.type) {
        case SortingStepType::COMPARE: opTag = "Compare"; opColor = YELLOW; break;
        case SortingStepType::SWAP: opTag = "Swap"; opColor = ORANGE; break;
        case SortingStepType::NO_SWAP: opTag = "No Swap"; opColor = LIGHTGRAY; break;
        case SortingStepType::PIVOT_SET: opTag = "Pivot"; opColor = PURPLE; break;
        case SortingStepType::SPLIT: opTag = "Split"; opColor = SKYBLUE; break;
        case SortingStepType::MERGE: opTag = "Merge"; opColor = GREEN; break;
        case SortingStepType::PASS_COMPLETE: opTag = "Pass Done"; opColor = LIME; break;
        case SortingStepType::SORTED: opTag = "Sorted!"; opColor = GOLD; break;
    }

    DrawRectangle(screenX + 15, screenY + 40, 110, 24, ColorAlpha(opColor, 0.3f));
    DrawRectangleLines(screenX + 15, screenY + 40, 110, 24, opColor);
    DrawText(opTag, screenX + 25, screenY + 44, 15, opColor);

    DrawText(currentStep.description.c_str(), screenX + 135, screenY + 44, 15, RAYWHITE);

    // Render elements as glowing magical crystals / bars
    for (size_t i = 0; i < count; ++i) {
        int barHeight = 40 + (arr[i] * maxBarHeight) / maxVal;
        int x = startX + static_cast<int>(i) * (elementWidth + 8);
        int y = startY - barHeight;

        Color fillColor = SKYBLUE;
        Color borderColor = BLUE;

        if (currentStep.type == SortingStepType::SORTED) {
            fillColor = GOLD;
            borderColor = YELLOW;
        } else if (i == currentStep.pivotIndex && (currentStep.type == SortingStepType::PIVOT_SET || currentStep.type == SortingStepType::COMPARE || currentStep.type == SortingStepType::SWAP)) {
            fillColor = PURPLE;
            borderColor = PINK;
        } else if (i == currentStep.index1 || i == currentStep.index2) {
            if (currentStep.type == SortingStepType::SWAP) {
                fillColor = ORANGE;
                borderColor = RED;
            } else if (currentStep.type == SortingStepType::COMPARE) {
                fillColor = YELLOW;
                borderColor = GOLD;
            }
        } else if (i >= currentStep.rangeStart && i <= currentStep.rangeEnd && (currentStep.type == SortingStepType::SPLIT || currentStep.type == SortingStepType::MERGE)) {
            fillColor = GREEN;
            borderColor = LIME;
        }

        // Draw crystal bar body
        DrawRectangle(x, y, elementWidth, barHeight, ColorAlpha(fillColor, 0.85f));
        DrawRectangleLines(x, y, elementWidth, barHeight, borderColor);

        // Draw value text inside/above bar
        DrawText(TextFormat("[%d]", arr[i]), x + (elementWidth / 2) - 12, y + 6, 14, DARKBLUE);
    }
}
