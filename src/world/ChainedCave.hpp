#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>
#include "../dsa/LinkedList.hpp"
#include "../dsa/Queue.hpp"
#include "../dsa/visualization/LinkedListVisualizer.hpp"
#include "../dsa/visualization/QueueVisualizer.hpp"

class Wizard;
class Enemy;
class AudioManager;
class ParticleSystem;
class QuestManager;

struct CrystalNodeData {
    int id{0};
    std::string name{"Crystal"};
    Color color{SKYBLUE};
    Vector2 worldPos{0.0f, 0.0f};

    bool operator==(const CrystalNodeData& other) const {
        return id == other.id;
    }
};

struct CaveEnemySpawn {
    std::string type;
    Vector2 pos;
};

class ChainedCave {
public:
    ChainedCave();

    void update(float deltaTime, Wizard& wizard, const class InputManager* inputManager,
                std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager,
                ParticleSystem& particles, QuestManager& questManager);

    void draw() const;

    Rectangle getBounds() const { return m_bounds; }
    Vector2 getEntrancePos() const { return m_entrancePos; }
    Vector2 getExitPos() const { return m_exitPos; }

    bool isPuzzleSolved() const { return m_puzzleSolved; }
    bool isEchoGateCleared() const { return m_gateCleared; }
    bool isGuardianSpawned() const { return m_guardianSpawned; }

    // Linked List puzzle actions
    void insertCrystal(AudioManager& audioManager);
    void removeCrystal(AudioManager& audioManager);
    void reverseCrystalChain(AudioManager& audioManager);

    const LinkedList<CrystalNodeData>& getCrystalChain() const { return m_crystalChain; }
    const LinkedListVisualizer& getListVisualizer() const { return m_listVisualizer; }
    const QueueVisualizer& getQueueVisualizer() const { return m_queueVisualizer; }

    bool isNearPuzzle(Vector2 pos) const;
    bool isNearGate(Vector2 pos) const;
    bool isNearExit(Vector2 pos) const;

private:
    void resolveCollisions(Wizard& wizard);
    void spawnQueuedEnemy(std::vector<std::unique_ptr<Enemy>>& enemies, AudioManager& audioManager, ParticleSystem& particles);

    Rectangle m_bounds;
    Vector2 m_entrancePos{220.0f, 650.0f};
    Vector2 m_exitPos{180.0f, 650.0f};

    std::vector<Rectangle> m_walls;
    std::vector<Vector2> m_waterPools;

    // Linked List Crystal Puzzle
    LinkedList<CrystalNodeData> m_crystalChain;
    bool m_puzzleSolved{false};
    bool m_isReversed{false};
    Vector2 m_puzzleCenter{620.0f, 420.0f};

    // Queue Echo Gate
    Queue<CaveEnemySpawn> m_enemyQueue;
    bool m_gateActive{false};
    bool m_gateCleared{false};
    float m_spawnIntervalTimer{0.0f};
    Vector2 m_gatePos{1050.0f, 450.0f};

    // Cave Guardian
    bool m_guardianSpawned{false};
    bool m_guardianDefeated{false};

    // Subterranean Harmony Crystal
    Vector2 m_caveCrystalPos{1450.0f, 680.0f};
    bool m_caveCrystalActivated{false};

    LinkedListVisualizer m_listVisualizer;
    QueueVisualizer m_queueVisualizer;
};
