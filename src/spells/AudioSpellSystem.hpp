#pragma once

#include "AudioNote.hpp"
#include "SpellManager.hpp"
#include "../dsa/Stack.hpp"
#include <vector>
#include <string>

class InputManager;
class AudioManager;
class Wizard;
class World;
class CombatSystem;

struct SpellHistoryRecord {
    std::string spellName;
    int manaSpent{0};
    Color themeColor{WHITE};
    float timestamp{0.0f};
};

class AudioSpellSystem {
public:
    AudioSpellSystem();

    void update(float deltaTime, const InputManager& input, Wizard& wizard, World& world, 
                CombatSystem& combatSystem, AudioManager& audioManager);

    // Current notes entered
    std::vector<AudioNote> getActiveSequence() const;
    const Spell* getMatchedPreviewSpell() const { return m_previewSpell; }

    // Spell Manager access
    const SpellManager& getSpellManager() const { return m_spellManager; }
    SpellManager& getSpellManager() { return m_spellManager; }

    // DSA Stack & Undo status
    const Stack<SpellHistoryRecord>& getSpellHistory() const { return m_spellHistory; }
    bool isShowingUndoVisualizer() const { return m_undoVisualizerTimer > 0.0f; }
    float getUndoVisualizerTimer() const { return m_undoVisualizerTimer; }
    const std::string& getLastPoppedSpell() const { return m_lastPoppedSpell; }
    const std::string& getEducationalMessage() const { return m_educationalMessage; }

    // Last cast spell feedback
    const std::string& getLastCastFeedback() const { return m_lastCastFeedback; }
    Color getLastCastFeedbackColor() const { return m_lastCastFeedbackColor; }
    float getFeedbackTimer() const { return m_feedbackTimer; }

private:
    void handleNoteInput(int raylibKey, AudioManager& audioManager, Wizard& wizard, World& world, CombatSystem& combatSystem);
    void evaluateSequence(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager);
    void undoLastAction(Wizard& wizard, AudioManager& audioManager, CombatSystem& combatSystem);

    SpellManager m_spellManager;

    // DSA Stacks
    Stack<AudioNote> m_noteInputStack;
    Stack<SpellHistoryRecord> m_spellHistory;

    Spell* m_previewSpell{nullptr};

    float m_sequenceInactivityTimer{0.0f};
    const float m_sequenceTimeout{4.0f};

    // Feedback and Educational HUD state
    float m_undoVisualizerTimer{0.0f};
    std::string m_lastPoppedSpell{""};
    std::string m_educationalMessage{""};
    bool m_firstUndoUsed{false};

    std::string m_lastCastFeedback{""};
    Color m_lastCastFeedbackColor{WHITE};
    float m_feedbackTimer{0.0f};
};
