#include "GameObject.h"

GameObject::GameObject(const char* spritePath, glm::vec3 pos) :
    spritePath{ spritePath }, pos{ pos } {}

const char* GameObject::getSpritePath() {
    return this->spritePath;
}

glm::vec3 GameObject::getPos() {
    return this->pos;
}