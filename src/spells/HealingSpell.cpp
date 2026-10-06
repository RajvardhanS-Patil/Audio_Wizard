#include "HealingSpell.hpp"
#include "../player/Wizard.hpp"
#include "../combat/CombatSystem.hpp"
#include "../audio/AudioManager.hpp"

HealingSpell::HealingSpell()
    : BaseSpell(
        "Healing Spell",
        "Channels forest resonance to restore 30 Health.",
        15,
        3.0f,
        SpellCombination({NotePitch::C, NotePitch::E, NotePitch::G}),
        Color{80, 240, 120, 255}
    ) {}

bool HealingSpell::execute(Wizard& wizard, World& /*world*/, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;

    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();

    audioManager.playSound("spell_heal");

    int healAmount = DamageSystem::calculateHealing(30, wizard.getStats().getSpellPowerMultiplier());
    wizard.getStats().restoreHealth(healAmount);

    combatSystem.getParticles().emitHealingSparkles(wizard.getPosition());
    combatSystem.addFloatingText({wizard.getPosition().x, wizard.getPosition().y - 30.0f},
                                 "+" + std::to_string(healAmount) + " HP", Color{80, 240, 120, 255}, 18);

    return true;
}
