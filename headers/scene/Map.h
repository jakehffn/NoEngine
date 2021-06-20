#pragma once

#include <vector>

#include "LogicObject.h"
#include "SpriteObject.h"

class Map {
public:
    std::vector<SpriteObject*> getSpriteObjects();
    std::vector<LogicObject*> getLogicObjects();

protected:
    std::vector<SpriteObject*> spriteObjects = std::vector<SpriteObject*>();
    std::vector<LogicObject*> logicObjects = std::vector<LogicObject*>();
};