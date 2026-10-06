#include "DarkAttackSpell.hpp"
#include "../player/Wizard.hpp"
#include "../combat/CombatSystem.hpp"
#include "../audio/AudioManager.hpp"

DarkAttackSpell::DarkAttackSpell()
    : BaseSpell(
        "Dark Attack",
        "Unleashes an abyssal sonic vortex dealing 48 dark damage.",
        25,
        2.0f,
        SpellCombination({NotePitch::A, NotePitch::C, NotePitch::E}),
        Color{170, 50, 240, 255}
    ) {}

bool DarkAttackSpell::execute(Wizard& wizard, World& /*world*/, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;

    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();

    audioManager.playSound("spell_dark");

    Vector2 dir = {static_cast<float>(wizard.getFacingDirection()), 0.0f};
    Vector2 spawnPos = {wizard.getPosition().x + dir.x * 20.0f, wizard.getPosition().y - 10.0f};

    combatSystem.spawnProjectile(spawnPos, dir, 320.0f, 48, Color{170, 50, 240, 255}, "dark");
    combatSystem.addFloatingText({wizard.getPosition().x, wizard.getPosition().y - 30.0f},
                                 "Dark Attack!", Color{170, 50, 240, 255}, 18);

    return true;
}
