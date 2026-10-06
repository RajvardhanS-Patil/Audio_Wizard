#pragma once

#include <raylib.h>
#include <vector>
#include <string>

class Wizard;
class AudioManager;
class ParticleSystem;
class QuestManager;

struct Tree {
    Vector2 position;
    float radius;
    bool corrupted;
};

struct Rock {
    Rectangle bounds;
};

struct HarmonyCrystal {
    Vector2 position;
    float radius;
    bool activated;
};

class World {
public:
    World();

    void update(Wizard& wizard, const class InputManager* inputManager);
    void update(Wizard& wizard, const class InputManager* inputManager,
                AudioManager& audioManager, ParticleSystem& particles,
                QuestManager& questManager);

    void draw() const;

    Rectangle getBounds() const { return m_bounds; }
    
    const std::string& getInteractionMessage() const { return m_interactionMessage; }
    float getInteractionMessageTimer() const { return m_messageTimer; }

    bool isGroveRestored() const { return m_groveRestored; }
    const HarmonyCrystal& getCrystal() const { return m_crystal; }

private:
    void resolveCollisions(Wizard& wizard);

    Rectangle m_bounds;
    
    std::vector<Tree> m_trees;
    std::vector<Rock> m_rocks;
    HarmonyCrystal m_crystal;
    
    std::string m_interactionMessage;
    float m_messageTimer{0.0f};
    bool m_showInteractionPrompt{false};
    bool m_groveRestored{false};
    float m_restorationAlpha{0.0f};
};
