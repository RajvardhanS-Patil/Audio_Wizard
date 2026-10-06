# Audio System

Music and audio form the core identity of the game.

- **Spell Casting:** C+E+G = Healing Spell, C+F+G = Fire Spell, D+F+A = Shield Spell, A+C+E = Dark Attack.
- **AudioSpellSystem:** Manages notes, combos, timings, and spell execution.

## Phase 3 Audio Architecture

In Phase 3, the Audio System was transformed into a decoupled, fully operational audio pipeline:

```
Keyboard Input (A, S, D, F, G, H, J)
      ↓
AudioSpellSystem (decides WHAT sound/note to trigger)
      ↓
AudioManager (decides HOW the sound is played, managed, and cached)
      ↓
Raylib Audio Subsystem (Device / Waves / Sounds / Music Streams)
```

### Musical Note Pitch Frequencies & Keyboard Mappings

| Keyboard Key | Musical Note | Frequency (Hz) | Theme Color | Audio Asset |
|:---:|:---:|:---:|:---:|:---:|
| `A` | **C4** | 261.63 Hz | Coral Red | `assets/audio/sfx/note_c.wav` |
| `S` | **D4** | 293.66 Hz | Orange | `assets/audio/sfx/note_d.wav` |
| `D` | **E4** | 329.63 Hz | Golden Yellow | `assets/audio/sfx/note_e.wav` |
| `F` | **F4** | 349.23 Hz | Emerald Green | `assets/audio/sfx/note_f.wav` |
| `G` | **G4** | 392.00 Hz | Cyan Blue | `assets/audio/sfx/note_g.wav` |
| `H` | **A4** | 440.00 Hz | Royal Indigo | `assets/audio/sfx/note_a.wav` |
| `J` | **B4** | 493.88 Hz | Violet Purple | `assets/audio/sfx/note_b.wav` |

### Audio Asset Directory Structure

```
assets/
  audio/
    music/
      forest_theme.wav        - Ambient pentatonic woodland background music loop
    ambience/
      forest_ambience.wav     - Gentle atmospheric wind and hum
    sfx/
      note_c.wav ... note_b.wav - Individual 16-bit PCM musical note samples
      enemy_detect.wav        - Menacing screech/growl alert sound
      enemy_attack.wav        - Kinetic claw swipe SFX
      enemy_hit.wav           - High-impact crunch SFX
      enemy_death.wav         - Corruption dissolving groan SFX
      crystal_restore.wav     - Celestial multi-frequency harmonic chord
      level_up.wav            - Triumphant ascending fanfare
    spells/
      fire.wav                - Blazing flame surge and impact
      heal.wav                - Ascending chime arpeggio (C5-E5-G5-C6)
      shield.wav              - Resonant shimmering barrier drone
      dark.wav                - Abyssal detuned low bass pulse
      fizzle.wav              - Discordant downward buzz
      undo.wav                - Ascending chime sweep for Stack Pop
```

### Fallback Synthesis & RAII Resource Management

`AudioManager` implements strict RAII:
- All `Sound` and `Music` instances are loaded into managed `std::unordered_map` caches.
- If relative asset paths vary across working directories, `AudioManager` contains an embedded procedural waveform synthesizer using Raylib's `LoadSoundFromWave`, guaranteeing that audio never fails or mutes.
- Destructor safely invokes `UnloadSound()`, `UnloadMusicStream()`, and `CloseAudioDevice()`.