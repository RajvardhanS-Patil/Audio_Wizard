# Progression System

Player attributes and progression are encapsulated within `src/player/PlayerStats.hpp`:

## Tracked Attributes

- **Health (HP):** Current / Max (starts at 100).
- **Mana (MP):** Current / Max (starts at 100).
- **Passive Mana Regeneration:** Recovers 4 MP per second continuously.
- **Attack Power:** Scales base damage for all spells.
- **Defense:** Mitigates incoming enemy melee damage.
- **Spell Power Multiplier:** Dynamic scalar:
  $$\text{Spell Power} = 1.0 + (\text{Level} - 1) \times 0.15 + (\text{DSA Mastery} \times 0.005)$$
- **Level & Experience (XP):**
  - Experience required for next level: $\text{Level} \times 100$.
  - Defeating the Corrupted Grove Beast awards $+100\text{ XP}$.
  - Cleansing the Harmony Crystal awards $+150\text{ XP}$.
- **Level Up Stat Gains:**
  - $+15$ Max HP (fully healed).
  - $+15$ Max MP (fully restored).
  - $+3$ Attack Power.
  - $+2$ Defense.
  - $+15\%$ Spell Power.
- **DSA Mastery:**
  - Progresses as DSA concepts are unlocked and utilized in gameplay.
  - Interacting with the Harmony Crystal awards $+15\%$ Stack Mastery.