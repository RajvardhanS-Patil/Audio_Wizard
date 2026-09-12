#pragma once

#include <raylib.h>

class InputManager {
public:
    InputManager();
    ~InputManager();

    // In Raylib, we can just poll real-time input state.
    void update();
    
    // Check if a key is currently being held down
    bool isKeyDown(int key) const;

    // Check if a key was pressed once in this frame
    bool isKeyPressed(int key) const;
};
