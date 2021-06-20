#include "TestMap.h"

TestMap::TestMap(Input* input, Clock* clock) {

    this->gameObjects.push_back(new BackgroundObject(clock));

    this->gameObjects.push_back(new PlayerObject(glm::vec3(200, 200, 0), input, clock));
}