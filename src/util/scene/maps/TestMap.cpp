#include "TestMap.h"

TestMap::TestMap() {
    this->gameObjects.push_back(PlayerObject(glm::vec3(200, 200, 0)));
}