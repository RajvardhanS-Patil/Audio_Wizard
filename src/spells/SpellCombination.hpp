#pragma once

#include "AudioNote.hpp"
#include <vector>
#include <string>
#include <initializer_list>

class SpellCombination {
public:
    SpellCombination() = default;

    SpellCombination(std::initializer_list<NotePitch> notes)
        : m_notes(notes) {}

    explicit SpellCombination(std::vector<NotePitch> notes)
        : m_notes(std::move(notes)) {}

    bool matches(const std::vector<NotePitch>& sequence) const {
        if (sequence.size() != m_notes.size()) return false;
        for (size_t i = 0; i < m_notes.size(); ++i) {
            if (sequence[i] != m_notes[i]) return false;
        }
        return true;
    }

    size_t length() const { return m_notes.size(); }
    const std::vector<NotePitch>& getNotes() const { return m_notes; }

    std::string toString() const {
        std::string result;
        for (size_t i = 0; i < m_notes.size(); ++i) {
            result += NoteUtils::pitchToString(m_notes[i]);
            if (i + 1 < m_notes.size()) {
                result += " + ";
            }
        }
        return result;
    }

private:
    std::vector<NotePitch> m_notes;
};
