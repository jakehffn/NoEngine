#pragma once

#include <unordered_map>
#include <tuple>
#include <utility>

#include <entt/entt.hpp>

#include "consts.h"

#include "Input.h"
#include "Model.h"
#include "Spacial.h"
#include "CameraController.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteState.h"

namespace create_entity {
    void VSCodeBackground(entt::registry& registry);
    void Player(entt::registry& registry, glm::vec3 pos);
    void Bag(entt::registry& registry, glm::vec3 pos);
};