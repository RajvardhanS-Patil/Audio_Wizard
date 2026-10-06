#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>
#include "../SortingAlgorithms.hpp"
#include "../DynamicArray.hpp"

enum class VisualizerSpeed {
    SLOW,   // 1.5s per step
    NORMAL, // 0.8s per step
    FAST    // 0.3s per step
};

class SortingVisualizer {
public:
    SortingVisualizer();

    void setArray(const DynamicArray<int>& array, const SortingAlgorithm& algorithm);
    void startVisualization();
    void reset();

    void update(float deltaTime);
    void draw(int screenX, int screenY, int width, int height) const;

    void cycleSpeed();
    VisualizerSpeed getSpeed() const { return m_speed; }
    const char* getSpeedName() const;

    bool isFinished() const { return m_finished; }
    size_t getCurrentStepIndex() const { return m_currentStepIndex; }
    size_t getTotalSteps() const { return m_steps.size(); }
    const std::string& getCurrentDescription() const { return m_currentDescription; }

private:
    std::vector<SortingStep> m_steps;
    size_t m_currentStepIndex{0};
    float m_stepTimer{0.0f};
    VisualizerSpeed m_speed{VisualizerSpeed::NORMAL};
    bool m_active{false};
    bool m_finished{false};
    std::string m_currentDescription{""};
    std::string m_algorithmName{""};

    float getStepDuration() const {
        switch (m_speed) {
            case VisualizerSpeed::SLOW: return 1.5f;
            case VisualizerSpeed::NORMAL: return 0.8f;
            case VisualizerSpeed::FAST: return 0.3f;
        }
        return 0.8f;
    }
};
