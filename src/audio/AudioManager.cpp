#include "AudioManager.hpp"

AudioManager::AudioManager() {
}

AudioManager::~AudioManager() {
    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}

void AudioManager::init() {
    InitAudioDevice();
    std::cout << "[AudioManager] Raylib Audio Device Initialized." << std::endl;
}

void AudioManager::update() {
    // Music streams need to be updated in Raylib
}

void AudioManager::playSound(const std::string& name) {
    // For Phase 1 we just output to console to simulate sound playing
    std::cout << "[AudioManager] Playing sound: " << name << std::endl;
}
