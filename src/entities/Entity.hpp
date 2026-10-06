#pragma once

#include <raylib.h>

class Entity {
public:
    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw() const = 0;

    Vector2 getPosition() const { return m_position; }
    void setPosition(Vector2 pos) { m_position = pos; }

protected:
    Vector2 m_position{ 0.0f, 0.0f };
};
