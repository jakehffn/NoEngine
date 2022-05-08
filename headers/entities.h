#pragma once

#include <unordered_map>
#include <tuple>
#include <utility>

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "consts.h"

#include "Input.h"
#include "Model.h"
#include "Spacial.h"
#include "CameraController.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteState.h"
#include "Velocity.h"
#include "Collision.h"
#include "Text.h"

namespace entities {

    void VSCodeBackground(entt::registry& registry);
    void Map1Background(entt::registry& registry);
    void IslandMapBackground(entt::registry& registry);
    void Player(entt::registry& registry, glm::vec3 pos);
    void BoxHead(entt::registry& registry, glm::vec3 pos);
    void Bag(entt::registry& registry, glm::vec3 pos);

    void CollisionBox(entt::registry& registry, glm::vec2 pos, glm::vec2 dim);

    void TextBox(entt::registry& registry, std::string text);

    Sprite createSprite(const char* spritesheetPath, int numSprites=1);

    // Tiled map name to entity create function map
    static std::unordered_map<std::string, void (*)(entt::registry&, glm::vec3)> create{
        {"Player", &Player}
    };

};