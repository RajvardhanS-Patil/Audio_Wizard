#pragma once

#include "Quest.hpp"
#include <vector>
#include <memory>

class QuestManager {
public:
    QuestManager();

    void update(float deltaTime);

    void onEnemyDefeated();
    void onCrystalRestored();
    void onEnterCave();
    void onEnterSortingArena();
    void onGuardianDefeated(); // Sorting Guardian
    void onEnterTreeOfKnowledge();
    void onTreeGuardianDefeated();
    void onEnterGraphCity();
    void onGraphCityRestored();

    const Quest* getActiveQuest() const;
    Quest* getActiveQuest();

    bool isQuestCompleted() const;
    const std::string& getNotification() const { return m_notification; }
    float getNotificationTimer() const { return m_notificationTimer; }

private:
    std::vector<std::unique_ptr<Quest>> m_quests;
    size_t m_currentQuestIndex{0};

    std::string m_notification{""};
    float m_notificationTimer{0.0f};
};
