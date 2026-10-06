#pragma once

#include <string>

enum class QuestState {
    NOT_STARTED,
    ACTIVE,
    ENEMY_DEFEATED,
    CRYSTAL_RESTORED,
    COMPLETED
};

class Quest {
public:
    Quest(std::string id, std::string title, std::string description)
        : m_id(std::move(id)),
          m_title(std::move(title)),
          m_description(std::move(description)),
          m_state(QuestState::NOT_STARTED) {}

    const std::string& getId() const { return m_id; }
    const std::string& getTitle() const { return m_title; }
    const std::string& getDescription() const { return m_description; }

    QuestState getState() const { return m_state; }
    void setState(QuestState newState) { m_state = newState; }

    std::string getObjectiveText() const {
        switch (m_state) {
            case QuestState::NOT_STARTED:
                return "Explore the Forest of Awakening";
            case QuestState::ACTIVE:
                return "Defeat the Corrupted Grove Beast";
            case QuestState::ENEMY_DEFEATED:
                return "Cleanse the Harmony Crystal with [E]";
            case QuestState::CRYSTAL_RESTORED:
            case QuestState::COMPLETED:
                return "Grove Restored! The Forest sings in harmony.";
        }
        return "";
    }

    bool isCompleted() const {
        return m_state == QuestState::COMPLETED || m_state == QuestState::CRYSTAL_RESTORED;
    }

private:
    std::string m_id;
    std::string m_title;
    std::string m_description;
    QuestState m_state{QuestState::NOT_STARTED};
};
