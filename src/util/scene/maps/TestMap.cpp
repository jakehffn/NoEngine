#include "TestMap.h"

TestMap::TestMap() {
    this->gameObjects.push_back(new PlayerObject(glm::vec3(200, 200, 0.0)));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(300, 200, 0.10)));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(400, 200, 0.20)));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(500, 200, 0.30)));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(600, 200, 0.40)));
    this->gameObjects.push_back(new PlayerObject(glm::vec3(700, 200, 0.5)));
}