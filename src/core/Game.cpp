#include "Game.hpp"

Game::Game() 
    : m_inputManager(std::make_unique<InputManager>())
    , m_audioManager(std::make_unique<AudioManager>())
{
    InitWindow(800, 600, "Audio Wizard");
    SetTargetFPS(60);

    // Setup visual placeholder for the wizard
    m_wizardPosition = { 400.0f, 300.0f };
    m_wizardRadius = 30.0f;
    m_wizardColor = BLUE;

    // Init Audio (Placeholder)
    m_audioManager->init();
}

Game::~Game() {
    m_audioManager.reset(); // Close audio device before window
    CloseWindow();
}

void Game::run() {
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    m_inputManager->update();
}

void Game::update(float deltaTime) {
    // Basic Movement logic using InputManager
    float speed = 200.0f; // pixels per second
    Vector2 movement = { 0.0f, 0.0f };

    if (m_inputManager->isKeyDown(KEY_W) || m_inputManager->isKeyDown(KEY_UP))
        movement.y -= speed;
    if (m_inputManager->isKeyDown(KEY_S) || m_inputManager->isKeyDown(KEY_DOWN))
        movement.y += speed;
    if (m_inputManager->isKeyDown(KEY_A) || m_inputManager->isKeyDown(KEY_LEFT))
        movement.x -= speed;
    if (m_inputManager->isKeyDown(KEY_D) || m_inputManager->isKeyDown(KEY_RIGHT))
        movement.x += speed;

    m_wizardPosition.x += movement.x * deltaTime;
    m_wizardPosition.y += movement.y * deltaTime;

    // Play a sound when space is pressed (simulating a basic spell)
    if (m_inputManager->isKeyPressed(KEY_SPACE)) {
        m_audioManager->playSound("spell");
    }

    m_audioManager->update();
}

void Game::render() {
    BeginDrawing();
    ClearBackground(Color{ 20, 20, 30, 255 }); // Dark fantasy background
    
    DrawCircleV(m_wizardPosition, m_wizardRadius, m_wizardColor);
    
    EndDrawing();
}
