#pragma once

#include <vector>

#include "LogicObject.h"
#include "SpriteObject.h"

#include "CameraController.h"

class Map {
public:
    CameraController* getCameraController();

    std::vector<LogicObject*> getLogicObjects();
    std::vector<SpriteObject*> getSpriteObjects();

protected:
    CameraController* cameraController;

    std::vector<LogicObject*> logicObjects = std::vector<LogicObject*>();
    std::vector<SpriteObject*> spriteObjects = std::vector<SpriteObject*>();
};