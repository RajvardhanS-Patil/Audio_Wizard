# Architecture

The game uses a modular, state-driven architecture built in C++ with custom OOP wrappers over Raylib.

## Core Systems

- **Game Core (`src/core/`):** Orchestrates the Game Loop (Input -> Update -> Render), State Management (`GameState.hpp`), Zone Transitions (`ZoneType`), and Camera boundaries.
- **Audio System (`src/audio/`):** `AudioManager` manages sound caching, miniaudio wave synthesis, cave ambience, and sound effect triggers.
- **Spell Architecture (`src/spells/`):**
  - `AudioNote`: Note pitch, key binding, frequency, and colors.
  - `SpellCombination`: Sequence matching and encapsulation.
  - `Spell`: Abstract base class with `canCast()`, `execute()`, and cooldown lifecycle.
  - `SpellFactory`: Factory Pattern for instantiating spells.
  - `SpellManager`: Spell registration and combo matching.
  - `AudioSpellSystem`: Handles real-time note input, spell evaluation, and Stack-based undo.
- **Special Abilities (`src/abilities/`):**
  - `ChainLink`: Linked List-powered ability tethering targets with electric chain beams (`[R]`).
  - `QueueSurge`: Queue-powered ability dispatching wave pulses in FIFO order (`[T]`).
- **Combat & Particles (`src/combat/`):**
  - `CombatSystem`: Manages projectiles, collision detection, floating damage numbers, and combo multipliers.
  - `DamageSystem`: Pure algorithmic damage calculations and defense mitigation.
  - `ParticleSystem`: High-speed pre-allocated particle pool.
  - `FloatingTextManager`: In-world floating numbers and combat notifications.
- **Entities & AI (`src/entities/`, `src/player/`):**
  - `Entity`: Base abstract class for positional game objects.
  - `Wizard`: Player entity with stats, shield barrier, hurt reactions, abilities, and rendering.
  - `Enemy`: Base enemy class with state machine AI.
  - `CorruptedCreature`: Forest beast enemy.
  - `ShadowBat`: Fast flying cave creature (`src/entities/ShadowBat.hpp`).
  - `StoneCrawler`: Heavy subterranean armored enemy (`src/entities/StoneCrawler.hpp`).
  - `ChainGuardian`: Miniboss boss protected by Linked List crystal shield (`src/entities/ChainGuardian.hpp`).
- **Quests (`src/quests/`):**
  - `Quest`: State machine for quest objectives.
  - `QuestManager`: Progression observer dispatching quest updates for Forest and Chained Cave.
- **Data Structures (`src/dsa/`):**
  - `Stack<T>`: Generic linked LIFO container powering the spell history and undo mechanic.
  - `LinkedList<T>`: Generic singly-linked list node container with full pointer reversal and positional operations (`src/dsa/LinkedList.hpp`).
  - `Queue<T>`: Generic linked FIFO queue container powering enemy wave spawning at the Echo Gate (`src/dsa/Queue.hpp`).
- **DSA Visualizers (`src/dsa/visualization/`):**
  - `LinkedListVisualizer`: Renders real-time node links and pointer operations (`insertFront`, `insertBack`, `reverse`).
  - `QueueVisualizer`: Renders FIFO front/rear queue alignment for enemy wave spawns.
- **World & Subterranean Zones (`src/world/`):**
  - `World`: Forest of Awakening environment, collisions, dynamic tree transformation, and Harmony Crystal restoration.
  - `ChainedCave`: Subterranean zone with broken chasm, glowing water pools, Echo Gate enemy queue, and Linked List crystal puzzle (`src/world/ChainedCave.hpp`).
- **UI & Educational Overlay (`src/ui/`):**
  - `HUD`: Simplified player-facing text, player stats, boss health bar, spell sequence runes, spellbook, quest tracker, abilities HUD, and Stack visualizer.
  - `DSAMasteryPanel`: Interactive panel (`[M]` / `[TAB]`) showing progress, gameplay usage, and unlocked abilities across Arrays, Stack, Linked List, and Queue.