#pragma once

#include <glm/glm.hpp>

#include "Map.h"
#include "Input.h"

#include "PlayerObject.h"

class TestMap : public Map {
public:
    TestMap(Input* input);
};