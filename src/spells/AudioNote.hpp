#pragma once

#include <string>
#include <raylib.h>

enum class NotePitch {
    C = 0,
    D,
    E,
    F,
    G,
    A,
    B,
    NONE
};

struct AudioNote {
    NotePitch pitch{NotePitch::NONE};
    char keyChar{'?'};
    std::string name{"None"};
    std::string soundName{""};
    float frequency{0.0f};
    Color color{WHITE};

    bool isValid() const { return pitch != NotePitch::NONE; }
};

namespace NoteUtils {
    inline AudioNote getNoteInfo(NotePitch pitch) {
        switch (pitch) {
            case NotePitch::C:
                return {NotePitch::C, 'A', "C", "note_c", 261.63f, Color{230, 60, 60, 255}};    // Coral Red
            case NotePitch::D:
                return {NotePitch::D, 'S', "D", "note_d", 293.66f, Color{245, 140, 40, 255}};   // Orange
            case NotePitch::E:
                return {NotePitch::E, 'D', "E", "note_e", 329.63f, Color{250, 220, 50, 255}};   // Golden Yellow
            case NotePitch::F:
                return {NotePitch::F, 'F', "F", "note_f", 349.23f, Color{60, 200, 90, 255}};    // Emerald Green
            case NotePitch::G:
                return {NotePitch::G, 'G', "G", "note_g", 392.00f, Color{50, 180, 240, 255}};   // Cyan Blue
            case NotePitch::A:
                return {NotePitch::A, 'H', "A", "note_a", 440.00f, Color{100, 110, 245, 255}};  // Royal Indigo
            case NotePitch::B:
                return {NotePitch::B, 'J', "B", "note_b", 493.88f, Color{190, 70, 240, 255}};   // Violet Purple
            default:
                return {NotePitch::NONE, '?', "None", "", 0.0f, GRAY};
        }
    }

    inline NotePitch keyToPitch(int key) {
        switch (key) {
            case KEY_A: return NotePitch::C;
            case KEY_S: return NotePitch::D;
            case KEY_D: return NotePitch::E;
            case KEY_F: return NotePitch::F;
            case KEY_G: return NotePitch::G;
            case KEY_H: return NotePitch::A;
            case KEY_J: return NotePitch::B;
            default:    return NotePitch::NONE;
        }
    }

    inline std::string pitchToString(NotePitch pitch) {
        return getNoteInfo(pitch).name;
    }
}
