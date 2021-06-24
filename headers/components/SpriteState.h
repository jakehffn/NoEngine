#pragma once

#include <unordered_map>

#include <Sprite.h>
#include "consts.h"

struct SpriteState {
    std::unordered_map<ENTITY_STATE, Sprite> stateMap{};

    ENTITY_STATE state{ENTITY_STATE::IDLE};
    ENTITY_STATE prevState{ENTITY_STATE::IDLE};
};
