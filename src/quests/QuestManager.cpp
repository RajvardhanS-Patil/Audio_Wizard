#include "QuestManager.hpp"

QuestManager::QuestManager() {
    auto q1 = std::make_unique<Quest>(
        "corrupted_grove",
        "The Corrupted Grove",
        "A foul corruption has taken root in the ancient grove. Defeat the Corrupted Creature and restore the Harmony Crystal to cleanse the land."
    );
    q1->setState(QuestState::ACTIVE);
    m_quests.push_back(std::move(q1));

    m_notification = "Quest Started: The Corrupted Grove";
    m_notificationTimer = 3.5f;
}

void QuestManager::update(float deltaTime) {
    if (m_notificationTimer > 0.0f) {
        m_notificationTimer -= deltaTime;
    }
}

void QuestManager::onEnemyDefeated() {
    Quest* q = getActiveQuest();
    if (q && q->getState() == QuestState::ACTIVE) {
        q->setState(QuestState::ENEMY_DEFEATED);
        m_notification = "Quest Update: Creature Defeated! Cleanse Harmony Crystal [E]";
        m_notificationTimer = 4.5f;
    }
}

void QuestManager::onCrystalRestored() {
    Quest* q = getActiveQuest();
    if (q && (q->getState() == QuestState::ENEMY_DEFEATED || q->getState() == QuestState::ACTIVE)) {
        q->setState(QuestState::COMPLETED);
        m_notification = "QUEST COMPLETE: The Corrupted Grove is Restored!";
        m_notificationTimer = 5.0f;
    }
}

void QuestManager::onEnterCave() {
    if (m_quests.size() < 2) {
        auto q2 = std::make_unique<Quest>(
            "repair_chained_cave",
            "Repair the Chained Cave",
            "Repair the crystal chain, restore the subterranean harmony, and defeat the Chain Guardian."
        );
        q2->setState(QuestState::ACTIVE);
        m_quests.push_back(std::move(q2));
        m_currentQuestIndex = 1;
        m_notification = "Quest Started: Repair the Chained Cave";
        m_notificationTimer = 4.0f;
    }
}

void QuestManager::onEnterSortingArena() {
    if (m_quests.size() < 3) {
        auto q3 = std::make_unique<Quest>(
            "restore_sorting_arena",
            "Restore the Sorting Arena",
            "Sort the crystal array using Bubble, Merge, and Quick Sort, and defeat the Sorting Serpent!"
        );
        q3->setState(QuestState::ACTIVE);
        m_quests.push_back(std::move(q3));
        m_currentQuestIndex = 2;
        m_notification = "Quest Started: Restore the Sorting Arena";
        m_notificationTimer = 4.0f;
    }
}

void QuestManager::onGuardianDefeated() {
    Quest* q = getActiveQuest();
    if (q) {
        q->setState(QuestState::COMPLETED);
        m_notification = "QUEST COMPLETE: Arena Restored & Harmony Preserved!";
        m_notificationTimer = 5.0f;
    }
}

void QuestManager::onEnterTreeOfKnowledge() {
    if (m_quests.size() < 4) {
        auto q4 = std::make_unique<Quest>(
            "restore_tree_knowledge",
            "Restore the Tree of Knowledge",
            "Discover hierarchical tree structures, complete the Inorder Traversal Challenge, and defeat the ancient Tree Guardian."
        );
        q4->setState(QuestState::ACTIVE);
        m_quests.push_back(std::move(q4));
        m_currentQuestIndex = 3;
        m_notification = "Quest Started: Restore the Tree of Knowledge";
        m_notificationTimer = 4.0f;
    }
}

void QuestManager::onTreeGuardianDefeated() {
    Quest* q = getActiveQuest();
    if (q) {
        q->setState(QuestState::COMPLETED);
        m_notification = "QUEST COMPLETE: Tree of Knowledge Restored!";
        m_notificationTimer = 5.0f;
    }
}

void QuestManager::onEnterGraphCity() {
    if (m_quests.size() < 5) {
        auto q5 = std::make_unique<Quest>(
            "restore_graph_city",
            "Restore Graph City",
            "Discover ancient vertices and edges. Restore the magical roads to connect the city's network!"
        );
        q5->setState(QuestState::ACTIVE);
        m_quests.push_back(std::move(q5));
        m_currentQuestIndex = 4;
        m_notification = "Quest Started: Restore Graph City";
        m_notificationTimer = 4.0f;
    }
}

void QuestManager::onGraphCityRestored() {
    Quest* q = getActiveQuest();
    if (q) {
        q->setState(QuestState::COMPLETED);
        m_notification = "QUEST COMPLETE: Graph City Network Restored!";
        m_notificationTimer = 5.0f;
    }
}

const Quest* QuestManager::getActiveQuest() const {
    if (m_currentQuestIndex < m_quests.size()) {
        return m_quests[m_currentQuestIndex].get();
    }
    return nullptr;
}

Quest* QuestManager::getActiveQuest() {
    if (m_currentQuestIndex < m_quests.size()) {
        return m_quests[m_currentQuestIndex].get();
    }
    return nullptr;
}

bool QuestManager::isQuestCompleted() const {
    const Quest* q = getActiveQuest();
    return q != nullptr && q->isCompleted();
}
