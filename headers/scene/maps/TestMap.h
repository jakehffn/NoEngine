#pragma once

#include <glm/glm.hpp>

#include "Map.h"
#include "Input.h"
#include "Clock.h"

#include "PlayerObject.h"
#include "BackgroundObject.h"

#include "PlayerCameraController.h"

class TestMap : public Map {
public:
    TestMap(Input* input, Clock* clock);
};