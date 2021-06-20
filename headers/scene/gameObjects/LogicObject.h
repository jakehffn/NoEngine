#pragma once

#include <glm/glm.hpp>

#include "Sprite.h"


class LogicObject {
public:
    LogicObject(glm::vec3 position);

    virtual void setPosition(glm::vec3 position);
    virtual void addPosition(glm::vec3 position);
    glm::vec3 getPosition() const;

    virtual void logic() = 0;

protected:
    glm::vec3 position;
};