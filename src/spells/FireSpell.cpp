#include "FireSpell.hpp"
#include "../player/Wizard.hpp"
#include "../combat/CombatSystem.hpp"
#include "../audio/AudioManager.hpp"

FireSpell::FireSpell()
    : BaseSpell(
        "Fire Spell",
        "Launches a searing bolt of elemental fire dealing 35 damage.",
        20,
        1.5f,
        SpellCombination({NotePitch::C, NotePitch::F, NotePitch::G}),
        Color{255, 120, 20, 255}
    ) {}

bool FireSpell::execute(Wizard& wizard, World& /*world*/, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;

    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();

    audioManager.playSound("spell_fire");

    Vector2 dir = {static_cast<float>(wizard.getFacingDirection()), 0.0f};
    Vector2 spawnPos = {wizard.getPosition().x + dir.x * 20.0f, wizard.getPosition().y - 10.0f};

    combatSystem.spawnProjectile(spawnPos, dir, 380.0f, 35, ORANGE, "fire");
    combatSystem.addFloatingText({wizard.getPosition().x, wizard.getPosition().y - 30.0f},
                                 "Fire Spell!", ORANGE, 18);

    return true;
}
