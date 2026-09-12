#pragma once

#include <string>
#include <map>
#include <iostream>
#include <raylib.h>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void init();
    void playSound(const std::string& name);
    void update();

private:
    // In Raylib, we will load Sound structs here later.
};
