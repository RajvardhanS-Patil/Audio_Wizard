# Gameplay Flow

## Phase 2 Flow (Prototype)
1. Start the game to view the Main Menu.
2. Enter the **Forest of Awakening** (Playable area).
3. Explore the forest using W, A, S, D.
4. Interact with the Harmony Crystal using 'E' to gain initial DSA Mastery.
5. Pause using ESC, and resume or quit using UI elements.

## Phase 3 Playable Vertical Slice Flow

Phase 3 introduces the first complete end-to-end playable vertical slice:

```
[START GAME] (Main Menu)
      ↓
[FOREST OF AWAKENING] (Smooth 2D Camera, Ambient Music, World Exploration)
      ↓
[EXPLORE THE CORRUPTED GROVE] (Corrupted trees, dark aura, quest begins)
      ↓
[ENCOUNTER ENEMY] (Corrupted Grove Beast detects Wizard, plays alert roar, initiates chase)
      ↓
[CAST MUSICAL AUDIO SPELLS]
  - Enter musical chords with A, S, D, F, G, H, J keys (Notes C, D, E, F, G, A, B)
  - Fire: C + F + G  |  Shield: D + F + A  |  Heal: C + E + G  |  Dark: A + C + E
  - Press [Q] to Undo Last Spell or note via DSA Stack (LIFO pop)
      ↓
[DAMAGE & DEFEAT ENEMY]
  - Projectiles collide, floating numbers pop, combo counter multiplies damage
  - Enemy dies with dissolving corruption particles and death audio
      ↓
[EARN REWARD & LEVEL UP]
  - +100 XP awarded, stats increase (+HP, +MP, +Spell Power)
      ↓
[RESTORE HARMONY CRYSTAL]
  - Approach glowing crystal, press [E]
  - Celestial chime plays, cleansing energy pulses outward
      ↓
[RESTORE THE CORRUPTED FOREST]
  - Corrupted grove transforms: trees bloom lush green, flowers sprout, dark mist vanishes
  - Quest "The Corrupted Grove" completes with Victory screen!
```