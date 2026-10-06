#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <raylib.h>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Prevent copy, allow move
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) noexcept;
    AudioManager& operator=(AudioManager&&) noexcept;

    void init();
    void playSound(const std::string& name);
    void playNote(const std::string& note, float volume);
    void playMusic(const std::string& name);
    void stopMusic();
    void update();

    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
    void setSoundVolume(const std::string& name, float volume);

    bool hasSound(const std::string& name) const;

private:
    void loadAllAssets();
    void loadSoundAsset(const std::string& name, const std::vector<std::string>& candidatePaths);
    void generateFallbackSound(const std::string& name);

    std::unordered_map<std::string, Sound> m_sounds;
    Music m_currentMusic;
    bool m_musicLoaded{false};
    bool m_audioReady{false};
    float m_masterVolume{1.0f};
    float m_musicVolume{0.7f};
};
