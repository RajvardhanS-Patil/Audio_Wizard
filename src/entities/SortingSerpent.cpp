#include "SortingSerpent.hpp"
#include "../player/Wizard.hpp"
#include "../audio/AudioManager.hpp"
#include "../combat/ParticleSystem.hpp"
#include <raymath.h>

SortingSerpent::SortingSerpent(Vector2 position) {
    m_position = position;
    m_health = 450.0f;
    m_maxHealth = 450.0f;

    // Initial unordered segment values
    m_segmentValues = { 50, 20, 80, 10, 40 };
    updateSegmentPositions();
}

Rectangle SortingSerpent::getBounds() const {
    return { m_position.x - 60.0f, m_position.y - 30.0f, 120.0f, 60.0f };
}

void SortingSerpent::takeDamage(int damage, AudioManager& audioManager, ParticleSystem& particles) {
    takeDamage(static_cast<float>(damage));
}

void SortingSerpent::takeDamage(float damage) {
    if (m_vulnerable) {
        m_health -= damage * 1.5f; // Bonus damage when vulnerable
    } else {
        m_health -= damage * 0.5f; // Reduced damage when shielded
    }
    if (m_health < 0.0f) m_health = 0.0f;
}

void SortingSerpent::updateSegmentPositions() {
    m_segments.clear();
    size_t count = m_segmentValues.size();
    for (size_t i = 0; i < count; ++i) {
        SerpentSegment seg;
        seg.value = m_segmentValues[i];
        // Serpent body snake arrangement around boss center
        float offsetX = (static_cast<float>(i) - static_cast<float>(count) / 2.0f) * 45.0f;
        float offsetY = sinf(m_position.x * 0.05f + static_cast<float>(i) * 0.8f) * 20.0f;
        seg.position = { m_position.x + offsetX, m_position.y + offsetY };
        seg.isPivot = (m_phase == BossPhase::QUICK_PHASE && i == count - 1);
        m_segments.push_back(seg);
    }
}

void SortingSerpent::updateAI(float deltaTime, Wizard& wizard, const World& /*world*/, AudioManager& audioManager, ParticleSystem& particles) {
    if (m_health <= 0.0f) return;

    // Movement: oscillate around arena center
    m_position.x += cosf(GetTime() * 1.2f) * 40.0f * deltaTime;
    m_position.y += sinf(GetTime() * 1.5f) * 25.0f * deltaTime;
    updateSegmentPositions();

    if (m_vulnerable) {
        m_vulnerableTimer -= deltaTime;
        if (m_vulnerableTimer <= 0.0f) {
            m_vulnerable = false;
        }
    } else {
        m_attackTimer += deltaTime;
        if (m_attackTimer >= 4.0f) {
            m_attackTimer = 0.0f;
            triggerBossAttack(wizard, particles, audioManager);
        }
    }

    checkPhaseTransition(particles, audioManager);
}

void SortingSerpent::triggerBossAttack(Wizard& wizard, ParticleSystem& particles, AudioManager& audioManager) {
    // Serpent fires un-sorted energy blast at wizard
    Vector2 dir = Vector2Normalize(Vector2Subtract(wizard.getPosition(), m_position));
    float dist = Vector2Distance(m_position, wizard.getPosition());
    if (dist < 280.0f) {
        wizard.getStats().takeDamage(20);
        particles.emitBurst(wizard.getPosition(), PURPLE, 12);
        audioManager.playNote("C3", 0.4f);
    }
}

void SortingSerpent::checkPhaseTransition(ParticleSystem& particles, AudioManager& audioManager) {
    float healthPct = m_health / m_maxHealth;
    if (healthPct <= 0.35f && m_phase != BossPhase::QUICK_PHASE) {
        m_phase = BossPhase::QUICK_PHASE;
        m_segmentValues = { 65, 15, 90, 30, 45, 70 };
        updateSegmentPositions();
        particles.emitBurst(m_position, PURPLE, 30);
        audioManager.playNote("G3", 0.6f);
    } else if (healthPct <= 0.70f && m_phase == BossPhase::BUBBLE_PHASE) {
        m_phase = BossPhase::MERGE_PHASE;
        m_segmentValues = { 40, 10, 75, 25, 60 };
        updateSegmentPositions();
        particles.emitBurst(m_position, GREEN, 25);
        audioManager.playNote("E3", 0.5f);
    }
}

bool SortingSerpent::applySortingStep(const std::string& method, AudioManager& audioManager, ParticleSystem& particles) {
    bool correctMethod = false;
    if (m_phase == BossPhase::BUBBLE_PHASE && (method == "Bubble" || method == "BubbleWave")) {
        correctMethod = true;
        m_bubbleSort.sort(m_segmentValues);
    } else if (m_phase == BossPhase::MERGE_PHASE && (method == "Merge" || method == "MergeMagic")) {
        correctMethod = true;
        m_mergeSort.sort(m_segmentValues);
    } else if (m_phase == BossPhase::QUICK_PHASE && (method == "Quick" || method == "QuickStrike")) {
        correctMethod = true;
        m_quickSort.sort(m_segmentValues);
    }

    if (correctMethod) {
        m_vulnerable = true;
        m_vulnerableTimer = 5.0f; // Vulnerability window opens
        updateSegmentPositions();
        particles.emitBurst(m_position, GOLD, 35);
        audioManager.playNote("C5", 0.6f);
        return true;
    } else {
        // Wrong sorting method applied -> serpent counters!
        particles.emitBurst(m_position, RED, 15);
        audioManager.playNote("C2", 0.5f);
        return false;
    }
}

void SortingSerpent::draw() const {
    if (m_health <= 0.0f) return;

    // Draw Serpent Segments
    for (size_t i = 0; i < m_segments.size(); ++i) {
        const auto& seg = m_segments[i];
        Color segColor = m_vulnerable ? GOLD : (m_phase == BossPhase::BUBBLE_PHASE ? SKYBLUE : (m_phase == BossPhase::MERGE_PHASE ? LIME : PURPLE));

        if (seg.isPivot) segColor = PINK;

        DrawCircleV(seg.position, 20.0f, segColor);
        DrawCircleLinesV(seg.position, 22.0f, WHITE);
        DrawText(TextFormat("%d", seg.value), seg.position.x - 10, seg.position.y - 7, 16, DARKBLUE);
    }

    // Draw Boss Head
    Color headColor = m_vulnerable ? YELLOW : RED;
    DrawCircleV(m_position, 28.0f, headColor);
    DrawCircleLinesV(m_position, 30.0f, GOLD);

    // Draw Boss Health Bar & Phase Label
    int barWidth = 180;
    int barHeight = 14;
    int barX = static_cast<int>(m_position.x - barWidth / 2);
    int barY = static_cast<int>(m_position.y - 45);

    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, static_cast<int>((m_health / m_maxHealth) * barWidth), barHeight, RED);
    DrawRectangleLines(barX, barY, barWidth, barHeight, GOLD);

    const char* phaseName = (m_phase == BossPhase::BUBBLE_PHASE) ? "Bubble Shield" : ((m_phase == BossPhase::MERGE_PHASE) ? "Merge Barrier" : "Quick Pivot Lock");
    DrawText(phaseName, barX + 30, barY - 16, 14, SKYBLUE);
}
