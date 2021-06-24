#pragma once

#include <entt/entt.hpp>

#include "consts.h"

#include "Input.h"
#include "Model.h"
#include "Spacial.h"
#include "CameraController.h"
#include "Animation.h"
#include "Sprite.h"





namespace create_entity {
    void addVSCodeBackground(entt::registry& registry);
    void addPlayer(entt::registry& registry, glm::vec3 pos);
    void addBag(entt::registry& registry, glm::vec3 pos);
};