#include "ShieldSpell.hpp"
#include "../player/Wizard.hpp"
#include "../combat/CombatSystem.hpp"
#include "../audio/AudioManager.hpp"

ShieldSpell::ShieldSpell()
    : BaseSpell(
        "Shield Spell",
        "Erects a protective musical barrier absorbing 35 damage for 8s.",
        10,
        4.5f,
        SpellCombination({NotePitch::D, NotePitch::F, NotePitch::A}),
        SKYBLUE
    ) {}

bool ShieldSpell::execute(Wizard& wizard, World& /*world*/, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;

    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();

    audioManager.playSound("spell_shield");

    wizard.activateShield(35.0f, 8.0f);

    combatSystem.getParticles().emitShieldSparks(wizard.getPosition());
    combatSystem.addFloatingText({wizard.getPosition().x, wizard.getPosition().y - 30.0f},
                                 "Shield Barrier (35)!", SKYBLUE, 18);

    return true;
}
