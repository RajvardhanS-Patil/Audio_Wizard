# Combat System

Combat integrates audio spell casting with DSA mastery. Bosses require specific algorithmic knowledge to defeat.

## Phase 3 Combat System Architecture

Phase 3 introduces the complete combat vertical slice:

```
Spell Execution
      ↓
CombatSystem (Manages active projectiles, collision detection, floating texts)
      ↓
DamageSystem (Decoupled damage arithmetic & defense mitigation calculations)
      ↓
Enemy / Wizard (Applies health deduction, shield absorption, hit stuns, XP rewards)
```

### Components

1. **DamageSystem (`src/combat/DamageSystem.hpp`, `DamageSystem.cpp`):**
   - Arithmetic decoupling for combat calculations:
     $$\text{Effective Damage} = \text{round}\left(\text{Raw Damage} \times \text{Spell Power} \times \text{Combo Multiplier} \times \frac{100}{100 + \text{Defense}}\right)$$
   - Minimum damage clamp of 1.
   - Healing scaling: $\text{Effective Healing} = \text{round}(\text{Base Healing} \times \text{Spell Power})$.

2. **CombatSystem (`src/combat/CombatSystem.hpp`, `CombatSystem.cpp`):**
   - Manages active `SpellProjectile` instances (`fire`, `dark`).
   - Projectile collision detection against enemy bounding boxes (`CheckCollisionCircleRec`).
   - Floating Combat Text (`FloatingTextManager`): displays damage numbers (`-35`), healing (`+30 HP`), shield absorption (`SHIELD BLOCKED!`), and combo counters (`COMBO x3!`).
   - High-performance `ParticleSystem`: pre-allocated 600-particle circular pool with zero per-frame heap allocations.
   - Combo System:
     - Each successful spell hit increments the combo counter ($x1, x2, x3...$).
     - Multiplier grants $+10\%$ damage per combo level (up to $2.5\times$).
     - Combo decay window: 4.5 seconds. Missing or fizzling a spell resets combo.

3. **First Enemy: Corrupted Grove Beast (`src/entities/CorruptedCreature.hpp`, `CorruptedCreature.cpp`):**
   - Implements the **State Pattern**:
     - `IDLE`: Pauses at waypoints, breathing corruption mist.
     - `PATROL`: Wanders between grove waypoints at 90 px/s.
     - `DETECT`: Alerts when Wizard enters detection radius (280 px), plays `enemy_detect` audio, displays exclamation mark.
     - `CHASE`: Pursues the Wizard aggressively at 160 px/s.
     - `ATTACK`: Lunges when within 48 px, deals 14 damage, triggers attack SFX and claw particles, enters 1.4s cooldown.
     - `TAKE_DAMAGE`: Flashes crimson/white, takes knockback, plays `enemy_hit` audio.
     - `DIE`: Plays `enemy_death` audio, releases dissolving corruption particles, shrinks over 0.85s.
     - `DEFEATED`: Grants +100 XP to Wizard, advances "The Corrupted Grove" quest!