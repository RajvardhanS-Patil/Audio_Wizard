#include "AudioSpellSystem.hpp"
#include "../core/InputManager.hpp"
#include "../audio/AudioManager.hpp"
#include "../player/Wizard.hpp"
#include "../world/World.hpp"
#include "../combat/CombatSystem.hpp"
#include <cstdio>
#include <algorithm>

AudioSpellSystem::AudioSpellSystem() = default;

std::vector<AudioNote> AudioSpellSystem::getActiveSequence() const {
    // Convert stack to vector (bottom to top for reading order)
    std::vector<AudioNote> topToBottom = m_noteInputStack.toVector();
    std::vector<AudioNote> ordered;
    ordered.reserve(topToBottom.size());
    for (auto it = topToBottom.rbegin(); it != topToBottom.rend(); ++it) {
        ordered.push_back(*it);
    }
    return ordered;
}

void AudioSpellSystem::update(float deltaTime, const InputManager& input, Wizard& wizard, World& world,
                              CombatSystem& combatSystem, AudioManager& audioManager) {
    // Update spell cooldowns
    m_spellManager.update(deltaTime);

    // Update timers
    if (m_feedbackTimer > 0.0f) {
        m_feedbackTimer -= deltaTime;
    }
    if (m_undoVisualizerTimer > 0.0f) {
        m_undoVisualizerTimer -= deltaTime;
    }

    // Sequence inactivity timeout
    if (!m_noteInputStack.isEmpty()) {
        m_sequenceInactivityTimer += deltaTime;
        if (m_sequenceInactivityTimer >= m_sequenceTimeout) {
            m_noteInputStack.clear();
            m_previewSpell = nullptr;
            m_sequenceInactivityTimer = 0.0f;
        }
    }

    // Check Note Key Inputs (A=C, S=D, D=E, F=F, G=G, H=A, J=B)
    const int noteKeys[] = {KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J};
    for (int key : noteKeys) {
        if (input.isKeyPressed(key)) {
            handleNoteInput(key, audioManager, wizard, world, combatSystem);
        }
    }

    // Key Q = Undo Last Spell (DSA Stack Pop)
    if (input.isKeyPressed(KEY_Q)) {
        undoLastAction(wizard, audioManager, combatSystem);
    }
}

void AudioSpellSystem::handleNoteInput(int raylibKey, AudioManager& audioManager, Wizard& wizard,
                                      World& world, CombatSystem& combatSystem) {
    NotePitch pitch = NoteUtils::keyToPitch(raylibKey);
    if (pitch == NotePitch::NONE) return;

    AudioNote note = NoteUtils::getNoteInfo(pitch);

    // Push note onto input Stack
    m_noteInputStack.push(note);
    m_sequenceInactivityTimer = 0.0f;

    // Play note audio feedback
    audioManager.playSound(note.soundName);

    // Visual note aura
    combatSystem.getParticles().emit(wizard.getPosition(), 8, note.color, 80.0f, 0.4f, 3.0f);

    // Evaluate current note sequence
    evaluateSequence(wizard, world, combatSystem, audioManager);
}

void AudioSpellSystem::evaluateSequence(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) {
    std::vector<AudioNote> orderedNotes = getActiveSequence();
    std::vector<NotePitch> pitches;
    pitches.reserve(orderedNotes.size());
    for (const auto& n : orderedNotes) {
        pitches.push_back(n.pitch);
    }

    Spell* matched = m_spellManager.findMatchingSpell(pitches);
    m_previewSpell = matched;

    if (matched != nullptr) {
        // A full combination was matched!
        if (matched->canCast(wizard.getStats())) {
            // Successful Cast!
            matched->execute(wizard, world, combatSystem, audioManager);

            // Record spell on Spell History Stack
            m_spellHistory.push({
                matched->getName(),
                matched->getManaCost(),
                matched->getThemeColor(),
                static_cast<float>(GetTime())
            });

            m_lastCastFeedback = matched->getName() + " Cast!";
            m_lastCastFeedbackColor = matched->getThemeColor();
            m_feedbackTimer = 2.2f;

            m_noteInputStack.clear();
            m_previewSpell = nullptr;
        } else if (wizard.getStats().getMana() < matched->getManaCost()) {
            // Insufficient Mana!
            audioManager.playSound("spell_fizzle");
            combatSystem.addFloatingText(wizard.getPosition(), "Not Enough Mana!", Color{120, 190, 255, 255}, 18);

            m_lastCastFeedback = "Not enough Mana (" + std::to_string(matched->getManaCost()) + " MP needed)";
            m_lastCastFeedbackColor = Color{120, 190, 255, 255};
            m_feedbackTimer = 2.5f;

            m_noteInputStack.clear();
            m_previewSpell = nullptr;
        } else if (!matched->isReady()) {
            // On Cooldown!
            audioManager.playSound("spell_fizzle");
            char buf[64];
            snprintf(buf, sizeof(buf), "%s on Cooldown (%.1fs)", matched->getName().c_str(), matched->getCurrentCooldown());
            combatSystem.addFloatingText(wizard.getPosition(), buf, GOLD, 16);

            m_lastCastFeedback = buf;
            m_lastCastFeedbackColor = GOLD;
            m_feedbackTimer = 2.0f;

            m_noteInputStack.clear();
            m_previewSpell = nullptr;
        }
    } else if (pitches.size() >= 3) {
        // 3 notes reached without matching any registered spell: Spell Fizzle!
        audioManager.playSound("spell_fizzle");
        combatSystem.addFloatingText(wizard.getPosition(), "Spell Fizzled!", GRAY, 18);
        combatSystem.resetCombo();

        m_lastCastFeedback = "Unknown combination. Spell Fizzled!";
        m_lastCastFeedbackColor = GRAY;
        m_feedbackTimer = 2.0f;

        m_noteInputStack.clear();
        m_previewSpell = nullptr;
    }
}

void AudioSpellSystem::undoLastAction(Wizard& wizard, AudioManager& audioManager, CombatSystem& combatSystem) {
    if (!m_noteInputStack.isEmpty()) {
        // Pop the most recent note from note stack
        AudioNote poppedNote = m_noteInputStack.top();
        m_noteInputStack.pop();

        audioManager.playSound("spell_undo");
        combatSystem.addFloatingText(wizard.getPosition(), "POP Note [" + poppedNote.name + "]", poppedNote.color, 16);

        m_lastPoppedSpell = "Note " + poppedNote.name;
        m_educationalMessage = "Stack follows LIFO: Last In, First Out.";
        m_undoVisualizerTimer = 4.0f;

        // Re-evaluate preview
        std::vector<AudioNote> ordered = getActiveSequence();
        std::vector<NotePitch> pitches;
        for (const auto& n : ordered) pitches.push_back(n.pitch);
        m_previewSpell = m_spellManager.findMatchingSpell(pitches);
    } else if (!m_spellHistory.isEmpty()) {
        // Pop the most recent cast spell from history stack
        SpellHistoryRecord popped = m_spellHistory.top();
        m_spellHistory.pop();

        // Refund mana spent
        wizard.getStats().restoreMana(popped.manaSpent);

        audioManager.playSound("spell_undo");
        combatSystem.getParticles().emitBurst(wizard.getPosition(), SKYBLUE, 20, 90.0f);
        combatSystem.addFloatingText(wizard.getPosition(), "UNDO: " + popped.spellName + " (+" + std::to_string(popped.manaSpent) + " MP)", SKYBLUE, 18);

        m_lastPoppedSpell = popped.spellName;
        m_educationalMessage = "Stack follows LIFO: Last In, First Out.";
        m_undoVisualizerTimer = 5.0f;
        m_firstUndoUsed = true;

        m_lastCastFeedback = "UNDO: " + popped.spellName + " refunded!";
        m_lastCastFeedbackColor = SKYBLUE;
        m_feedbackTimer = 2.5f;
    } else {
        combatSystem.addFloatingText(wizard.getPosition(), "Stack Empty (Nothing to Undo)", LIGHTGRAY, 14);
    }
}
