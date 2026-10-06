# Game States

Managed via State Pattern in `src/core/GameState.hpp`:

## Implemented States (Phase 3)

1. **`MAIN_MENU`:**
   - Title screen, play and quit options, audio initialization, and control guide.
2. **`EXPLORATION`:**
   - Active gameplay in the Forest of Awakening.
   - Smooth camera tracking, player movement, musical note input, real-time combat, and world interaction.
3. **`PAUSE`:**
   - Overlaid semi-transparent menu toggled with `[ESC]`.
   - Offers Resume, Settings, and Quit.
4. **`VICTORY`:**
   - Triggered upon cleansing the Harmony Crystal and restoring the Corrupted Grove.
   - Celebrates vertical slice completion with option to resume open exploration.
5. **`GAME_OVER`:**
   - Triggered if Wizard's HP reaches 0 in combat.
   - Allows rising again (`[ENTER]`) or returning to menu (`[ESC]`).

## Future States (Phases 4+)
- `WORLD_MAP`
- `BOSS_BATTLE`
- `CHALLENGE_MODE`
- `INTERVIEW_MODE`