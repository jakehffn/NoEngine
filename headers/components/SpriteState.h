#pragma once

#include <unordered_map>
#include <tuple>

#include "Sprite.h"
#include "Animation.h"

#include "consts.h"

struct SpriteState {
    std::unordered_map<ENTITY_STATE, std::tuple<Sprite, Animation>> stateMap{};

    ENTITY_STATE state{ENTITY_STATE::IDLE};
    ENTITY_STATE prevState{ENTITY_STATE::IDLE};
};
