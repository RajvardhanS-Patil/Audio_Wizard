#include "QueueSurge.hpp"
#include "../player/Wizard.hpp"
#include "../player/PlayerStats.hpp"
#include "../combat/CombatSystem.hpp"
#include "../audio/AudioManager.hpp"
#include <algorithm>

QueueSurge::QueueSurge() = default;

bool QueueSurge::canActivate(const PlayerStats& stats) const {
    return m_unlocked && m_currentCooldown <= 0.0f && stats.getMana() >= m_manaCost;
}

void QueueSurge::activate(Wizard& wizard, CombatSystem& combatSystem, AudioManager& audioManager) {
    if (!canActivate(wizard.getStats())) return;

    wizard.getStats().consumeMana(m_manaCost);
    m_currentCooldown = m_cooldownDuration;
    m_elapsed = 0.0f;

    Vector2 dir = {static_cast<float>(wizard.getFacingDirection()), 0.0f};
    Vector2 origin = {wizard.getPosition().x + dir.x * 20.0f, wizard.getPosition().y - 10.0f};

    // Enqueue 3 burst pulses into FIFO Queue with stagger delays
    m_pulseQueue.enqueue({origin, dir, 0.0f, GOLD, 26});
    m_pulseQueue.enqueue({origin, dir, 0.12f, SKYBLUE, 26});
    m_pulseQueue.enqueue({origin, dir, 0.24f, MAGENTA, 32});

    audioManager.playSound("queue_surge");
    combatSystem.addFloatingText(wizard.getPosition(), "Queue Surge (FIFO Salvo)!", GOLD, 16);
}

void QueueSurge::update(float deltaTime, CombatSystem& combatSystem, AudioManager& /*audioManager*/) {
    if (m_currentCooldown > 0.0f) {
        m_currentCooldown = std::max(0.0f, m_currentCooldown - deltaTime);
    }

    if (m_pulseQueue.empty()) return;

    m_elapsed += deltaTime;

    // Check if the front element is ready to be dequeued and fired
    while (!m_pulseQueue.empty() && m_elapsed >= m_pulseQueue.front().delay) {
        SurgePulse pulse = m_pulseQueue.front();
        m_pulseQueue.dequeue(); // FIFO Pop from front!

        combatSystem.spawnProjectile(pulse.origin, pulse.direction, 420.0f, pulse.damage, pulse.color, "surge");
        combatSystem.getParticles().emit(pulse.origin, 10, pulse.color, 100.0f, 0.3f, 3.0f);
    }
}
