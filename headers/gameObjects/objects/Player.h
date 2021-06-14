#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    Player(Scene* scene, glm::vec3 pos);

    void setPos(glm::vec3 pos);
    void move(glm::vec3 movement);

private:
    Instance* instance;
};