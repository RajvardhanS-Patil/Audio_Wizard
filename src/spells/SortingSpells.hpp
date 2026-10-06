#pragma once

#include "BaseSpell.hpp"

// 1. Bubble Wave Spell (C -> E -> G)
class BubbleWaveSpell : public BaseSpell {
public:
    BubbleWaveSpell() 
        : BaseSpell("Bubble Wave", "Unleashes Bubble Sort wave comparing and swapping nearby elements.", 20, 4.0f,
                    SpellCombination({ NotePitch::C, NotePitch::E, NotePitch::G }), SKYBLUE) {}

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};

// 2. Merge Magic Spell (D -> F -> A)
class MergeMagicSpell : public BaseSpell {
public:
    MergeMagicSpell() 
        : BaseSpell("Merge Magic", "Splits and merges magical energy beams to combine sorted forces.", 30, 6.0f,
                    SpellCombination({ NotePitch::D, NotePitch::F, NotePitch::A }), LIME) {}

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};

// 3. Quick Strike Spell (C -> F -> G)
class QuickStrikeSpell : public BaseSpell {
public:
    QuickStrikeSpell() 
        : BaseSpell("Quick Strike", "Designates a pivot target and partitions energy for rapid strikes.", 35, 8.0f,
                    SpellCombination({ NotePitch::C, NotePitch::F, NotePitch::G }), PURPLE) {}

    bool execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) override;
};
