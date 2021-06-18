#pragma once

#include <glm/glm.hpp>

class GameObject {
public:
    GameObject(const char* spritePath, glm::vec3 pos);

    const char* getSpritePath();
    glm::vec3 getPos();

    virtual void logic() = 0;

protected:
    glm::vec3 pos;
    const char* spritePath;
};