#pragma once

#include <vector>

#include "GameObject.h"

class Map {
public:
    std::vector<GameObject> getGameObjects();

protected:
    std::vector<GameObject> gameObjects = std::vector<GameObject>();
};