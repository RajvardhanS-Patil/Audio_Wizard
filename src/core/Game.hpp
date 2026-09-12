#pragma once

#include <raylib.h>
#include <memory>
#include "InputManager.hpp"
#include "../audio/AudioManager.hpp"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<AudioManager> m_audioManager;

    // Phase 1 visual placeholder state
    Vector2 m_wizardPosition;
    float m_wizardRadius;
    Color m_wizardColor;
};
