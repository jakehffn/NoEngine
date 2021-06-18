#include "TestMap.h"

TestMap::TestMap(Input* input) {
    this->gameObjects.push_back(new PlayerObject(glm::vec3(200, 200, 0.0), input));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(300, 200, 0.10), input));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(400, 200, 0.20), input));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(500, 200, 0.30), input));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(600, 200, 0.40), input));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(700, 200, 0.5), input));
}