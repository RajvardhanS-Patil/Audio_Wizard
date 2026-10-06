#include "InputManager.hpp"

InputManager::InputManager() = default;
InputManager::~InputManager() = default;

void InputManager::update() {
    // Raylib's state is updated internally, 
    // but having an update function allows us to build custom input buffering/combo logic later.
}

bool InputManager::isKeyDown(int key) const {
    return ::IsKeyDown(key);
}

bool InputManager::isKeyPressed(int key) const {
    return ::IsKeyPressed(key);
}

bool InputManager::isMouseButtonPressed(int button) const {
    return ::IsMouseButtonPressed(button);
}

Vector2 InputManager::getMousePosition() const {
    return ::GetMousePosition();
}
