#include "Map.h"

std::vector<SpriteObject*> Map::getSpriteObjects() {
    return this->spriteObjects;
}

std::vector<LogicObject*> Map::getLogicObjects() {
    return this->logicObjects;
}