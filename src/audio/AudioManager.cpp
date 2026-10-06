#include "AudioManager.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() {
    if (m_musicLoaded) {
        StopMusicStream(m_currentMusic);
        UnloadMusicStream(m_currentMusic);
        m_musicLoaded = false;
    }

    std::unordered_set<void*> unloadedBuffers;
    for (auto& pair : m_sounds) {
        if (pair.second.stream.buffer != nullptr &&
            unloadedBuffers.insert(pair.second.stream.buffer).second) {
            UnloadSound(pair.second);
        }
    }
    m_sounds.clear();

    if (m_audioReady && IsAudioDeviceReady()) {
        CloseAudioDevice();
        m_audioReady = false;
    }
}

AudioManager::AudioManager(AudioManager&& other) noexcept
    : m_sounds(std::move(other.m_sounds)),
      m_currentMusic(other.m_currentMusic),
      m_musicLoaded(other.m_musicLoaded),
      m_audioReady(other.m_audioReady),
      m_masterVolume(other.m_masterVolume),
      m_musicVolume(other.m_musicVolume) {
    other.m_musicLoaded = false;
    other.m_audioReady = false;
}

AudioManager& AudioManager::operator=(AudioManager&& other) noexcept {
    if (this != &other) {
        if (m_musicLoaded) {
            StopMusicStream(m_currentMusic);
            UnloadMusicStream(m_currentMusic);
        }
        std::unordered_set<void*> unloadedBuffers;
        for (auto& pair : m_sounds) {
            if (pair.second.stream.buffer != nullptr &&
                unloadedBuffers.insert(pair.second.stream.buffer).second) {
                UnloadSound(pair.second);
            }
        }
        m_sounds.clear();
        if (m_audioReady && IsAudioDeviceReady()) {
            CloseAudioDevice();
        }

        m_sounds = std::move(other.m_sounds);
        m_currentMusic = other.m_currentMusic;
        m_musicLoaded = other.m_musicLoaded;
        m_audioReady = other.m_audioReady;
        m_masterVolume = other.m_masterVolume;
        m_musicVolume = other.m_musicVolume;

        other.m_musicLoaded = false;
        other.m_audioReady = false;
    }
    return *this;
}

void AudioManager::init() {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }
    m_audioReady = IsAudioDeviceReady();
    if (m_audioReady) {
        std::cout << "[AudioManager] Raylib Audio Device Initialized Successfully." << std::endl;
        loadAllAssets();
        playMusic("forest_theme");
    } else {
        std::cerr << "[AudioManager] Warning: Failed to initialize audio device." << std::endl;
    }
}

void AudioManager::loadSoundAsset(const std::string& name, const std::vector<std::string>& candidatePaths) {
    if (!m_audioReady) return;

    for (const auto& path : candidatePaths) {
        if (FileExists(path.c_str())) {
            Sound sound = LoadSound(path.c_str());
            if (sound.frameCount > 0) {
                m_sounds[name] = sound;
                return;
            }
        }
    }

    // If file could not be loaded from disk, synthesize wave as fallback
    generateFallbackSound(name);
}

void AudioManager::generateFallbackSound(const std::string& name) {
    if (!m_audioReady) return;

    int sampleRate = 44100;
    int sampleCount = sampleRate / 3; // ~0.33s
    float freq = 440.0f;

    if (name == "note_c") freq = 261.63f;
    else if (name == "note_d") freq = 293.66f;
    else if (name == "note_e") freq = 329.63f;
    else if (name == "note_f") freq = 349.23f;
    else if (name == "note_g") freq = 392.00f;
    else if (name == "note_a") freq = 440.00f;
    else if (name == "note_b") freq = 493.88f;
    else if (name == "spell_fire" || name == "fire") freq = 220.0f;
    else if (name == "spell_heal" || name == "heal") freq = 587.33f;
    else if (name == "spell_shield" || name == "shield") freq = 349.23f;
    else if (name == "spell_dark" || name == "dark") freq = 130.81f;

    short* data = new short[sampleCount];
    for (int i = 0; i < sampleCount; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float env = 1.0f - (static_cast<float>(i) / sampleCount);
        float sample = std::sin(2.0f * 3.14159265f * freq * t) * env;
        data[i] = static_cast<short>(sample * 28000.0f);
    }

    Wave wave{};
    wave.frameCount = sampleCount;
    wave.sampleRate = sampleRate;
    wave.sampleSize = 16;
    wave.channels = 1;
    wave.data = data;

    Sound sound = LoadSoundFromWave(wave);
    delete[] data;

    if (sound.frameCount > 0) {
        m_sounds[name] = sound;
    }
}

void AudioManager::loadAllAssets() {
    // Musical note sounds
    std::vector<std::string> noteNames = {"c", "d", "e", "f", "g", "a", "b"};
    for (const auto& n : noteNames) {
        std::string soundName = "note_" + n;
        loadSoundAsset(soundName, {
            "assets/audio/sfx/" + soundName + ".wav",
            "../assets/audio/sfx/" + soundName + ".wav",
            "../../assets/audio/sfx/" + soundName + ".wav"
        });
    }

    // Spell sounds
    std::vector<std::string> spells = {"fire", "heal", "shield", "dark", "fizzle", "undo"};
    for (const auto& sp : spells) {
        std::string soundName = "spell_" + sp;
        loadSoundAsset(soundName, {
            "assets/audio/spells/" + sp + ".wav",
            "../assets/audio/spells/" + sp + ".wav",
            "../../assets/audio/spells/" + sp + ".wav"
        });
        // Create aliases like "fire", "heal"
        if (m_sounds.find(soundName) != m_sounds.end()) {
            m_sounds[sp] = m_sounds[soundName];
        }
    }
    // Generic "spell" fallback
    if (m_sounds.find("spell_fire") != m_sounds.end()) {
        m_sounds["spell"] = m_sounds["spell_fire"];
    }

    // Combat, Creature & Cave sounds
    std::vector<std::string> sfxNames = {
        "enemy_detect", "enemy_attack", "enemy_hit", "enemy_death", "crystal_restore", "level_up",
        "chain_move", "chain_link", "queue_surge", "bat_screech", "crawler_slam",
        "guardian_roar", "guardian_vulnerable", "linked_list_insert", "linked_list_reverse",
        "queue_enqueue", "queue_dequeue"
    };
    for (const auto& s : sfxNames) {
        loadSoundAsset(s, {
            "assets/audio/sfx/" + s + ".wav",
            "../assets/audio/sfx/" + s + ".wav",
            "../../assets/audio/sfx/" + s + ".wav"
        });
    }

    std::cout << "[AudioManager] Loaded " << m_sounds.size() << " sound assets." << std::endl;
}

void AudioManager::playSound(const std::string& name) {
    if (!m_audioReady) return;

    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        PlaySound(it->second);
    } else {
        std::cout << "[AudioManager] Sound not loaded: " << name << std::endl;
    }
}

void AudioManager::playNote(const std::string& note, float volume) {
    if (!m_audioReady || note.empty()) return;
    
    std::string name = "note_";
    name += static_cast<char>(std::tolower(note[0]));
    
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        SetSoundVolume(it->second, volume);
        PlaySound(it->second);
    } else {
        std::cout << "[AudioManager] Note sound not loaded: " << name << std::endl;
    }
}

void AudioManager::playMusic(const std::string& name) {
    if (!m_audioReady) return;

    if (m_musicLoaded) {
        StopMusicStream(m_currentMusic);
        UnloadMusicStream(m_currentMusic);
        m_musicLoaded = false;
    }

    std::vector<std::string> candidates = {
        "assets/audio/music/" + name + ".wav",
        "../assets/audio/music/" + name + ".wav",
        "../../assets/audio/music/" + name + ".wav",
        "assets/audio/ambience/" + name + ".wav",
        "../assets/audio/ambience/" + name + ".wav",
        "../../assets/audio/ambience/" + name + ".wav"
    };

    for (const auto& path : candidates) {
        if (FileExists(path.c_str())) {
            m_currentMusic = LoadMusicStream(path.c_str());
            if (m_currentMusic.ctxData != nullptr) {
                m_currentMusic.looping = true;
                SetMusicVolume(m_currentMusic, m_musicVolume);
                PlayMusicStream(m_currentMusic);
                m_musicLoaded = true;
                std::cout << "[AudioManager] Playing music stream: " << path << std::endl;
                return;
            }
        }
    }
}

void AudioManager::stopMusic() {
    if (m_musicLoaded) {
        StopMusicStream(m_currentMusic);
    }
}

void AudioManager::update() {
    if (m_musicLoaded) {
        UpdateMusicStream(m_currentMusic);
    }
}

void AudioManager::setMasterVolume(float volume) {
    m_masterVolume = volume;
    SetMasterVolume(volume);
}

void AudioManager::setMusicVolume(float volume) {
    m_musicVolume = volume;
    if (m_musicLoaded) {
        SetMusicVolume(m_currentMusic, volume);
    }
}

void AudioManager::setSoundVolume(const std::string& name, float volume) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        SetSoundVolume(it->second, volume);
    }
}

bool AudioManager::hasSound(const std::string& name) const {
    return m_sounds.find(name) != m_sounds.end();
}
