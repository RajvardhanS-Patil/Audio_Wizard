#include "SortingSpells.hpp"
#include "../player/Wizard.hpp"
#include "../combat/CombatSystem.hpp"
#include "../audio/AudioManager.hpp"

bool BubbleWaveSpell::execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;
    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();
    wizard.castBubbleWave(combatSystem.getEnemies(), combatSystem.getParticles(), audioManager);
    audioManager.playNote("C4", 0.4f);
    audioManager.playNote("E4", 0.4f);
    audioManager.playNote("G4", 0.5f);
    return true;
}

bool MergeMagicSpell::execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;
    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();
    wizard.castMergeMagic(combatSystem.getEnemies(), combatSystem.getParticles(), audioManager);
    audioManager.playNote("D4", 0.4f);
    audioManager.playNote("F4", 0.4f);
    audioManager.playNote("A4", 0.6f);
    return true;
}

bool QuickStrikeSpell::execute(Wizard& wizard, World& world, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canCast(wizard.getStats())) return false;
    wizard.getStats().consumeMana(m_manaCost);
    triggerCooldown();
    wizard.castQuickStrike(combatSystem.getEnemies(), combatSystem.getParticles(), audioManager);
    audioManager.playNote("C4", 0.3f);
    audioManager.playNote("F4", 0.3f);
    audioManager.playNote("G4", 0.5f);
    return true;
}
