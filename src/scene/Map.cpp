#include "Map.h"

CameraController* Map::getCameraController() {
    return this->cameraController;
}

std::vector<LogicObject*> Map::getLogicObjects() {
    return this->logicObjects;
}

std::vector<SpriteObject*> Map::getSpriteObjects() {
    return this->spriteObjects;
}