# Spell System

Spells are cast using sequences of musical notes.

- **Validation:** Checks user input against known spell recipes.
- **DSA Spells:** Unlock advanced spells based on mastered DSA concepts (e.g., Undo Spell from Stacks).

## Phase 3 Audio Spell Architecture

In Phase 3, spells are encapsulated using OOP principles with an abstract `Spell` class, a base `BaseSpell` implementation, concrete spells (`FireSpell`, `HealingSpell`, `ShieldSpell`, `DarkAttackSpell`), and a `SpellFactory` / `SpellManager`.

### Spell Class Hierarchy

```
Spell (Abstract Interface)
  └── BaseSpell (Manages cooldown, mana cost, combinations, theme color)
        ├── FireSpell        (C + F + G | 20 MP | 1.5s CD | 35 Fire Damage Projectile)
        ├── HealingSpell     (C + E + G | 15 MP | 3.0s CD | +30 HP & Nature Aura)
        ├── ShieldSpell      (D + F + A | 10 MP | 4.5s CD | 35 Point Barrier for 8s)
        └── DarkAttackSpell  (A + C + E | 25 MP | 2.0s CD | 48 Void Damage Projectile)
```

### Registered Spell Combinations & Gameplay Effects

| Spell Name | Note Combination | Keyboard Keys | Mana Cost | Cooldown | Effect |
|:---|:---:|:---:|:---:|:---:|:---|
| **Fire Spell** | `C + F + G` | `A` + `F` + `G` | 20 MP | 1.5s | Hurls blazing projectile towards facing target; deals 35 base fire damage. |
| **Healing Spell** | `C + E + G` | `A` + `D` + `G` | 15 MP | 3.0s | Channels harmonic nature aura; restores 30 HP with sparkling emerald particles. |
| **Shield Spell** | `D + F + A` | `S` + `F` + `H` | 10 MP | 4.5s | Conjures rotating sapphire energy ring absorbing 35 damage for 8 seconds. |
| **Dark Attack** | `A + C + E` | `H` + `A` + `D` | 25 MP | 2.0s | Fires a piercing shadow orb dealing 48 dark damage with void tendrils. |

### Spell Recognition & Execution Flow

```
1. Player presses note key (e.g. 'A')
2. AudioNote registered, frequency tone played via AudioManager
3. Note pushed to m_noteInputStack; rune displayed on HUD
4. AudioSpellSystem evaluates active note sequence
5. If sequence matches known recipe:
     a. Check player mana >= spell->getManaCost()
     b. Check spell->isReady() (cooldown <= 0)
     c. If valid: Execute spell, deduct mana, trigger cooldown, push to m_spellHistory Stack, increment combo, clear note sequence.
     d. If out of mana: Play fizzle SFX, display "Not Enough Mana!" floating text.
     e. If on cooldown: Play fizzle SFX, display "On Cooldown (X.Xs)".
6. If sequence reaches 3 notes and does not match:
     Play fizzle SFX, display "Spell Fizzled!", reset combo, clear note sequence.
```

### Spell Cooldowns & Mana System

- **Mana Pool:** Wizard starts with 100 Mana.
- **Passive Mana Regeneration:** Wizard passively recovers ~4 Mana per second via `PlayerStats::update()`.
- **Cooldown Indicators:** Displayed in real-time in the HUD Spellbook panel (`[READY]` or `[1.2s]`).