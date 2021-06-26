#pragma once

#include <unordered_map>
#include <tuple>
#include <utility>

#include "Sprite.h"
#include "Animation.h"

#include "consts.h"

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Bad hash; prone to need rehashing, but for this limited use-case, works fine
        return h1 ^ h2;  
    }
};

using SpriteStatePair = std::pair<entity_c::ENTITY_STATE, entity_c::ENTITY_DIR>;

struct SpriteState {    
    std::unordered_map<SpriteStatePair, std::tuple<Sprite, Animation>, pair_hash> stateMap{};

    SpriteStatePair state{std::make_pair(entity_c::IDLE, entity_c::DOWN)};
};
