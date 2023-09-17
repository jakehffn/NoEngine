#pragma once

#include <set>
#include <algorithm>

#include <entt/entt.hpp>
#include <component_grid.hpp>

#include "velocity.hpp"
#include "player_control.hpp"
#include "spacial.hpp"
#include "outline.hpp"
#include "camera.hpp"
#include "renderable.hpp"
#include "cursor.hpp"
#include "left_clicked.hpp"
#include "right_clicked.hpp"
#include "hovered.hpp"
#include "collision.hpp"
#include "collidable.hpp"
#include "interactable.hpp"
#include "interaction.hpp"
#include "interacter.hpp"
#include "persistent.hpp"

#include "map_loader.hpp"

#include "system.hpp"

#include "input.hpp"
#include "resource_loader.hpp"

#include "debug_timer.hpp"

class InputSystem : public System {
public:
    InputSystem(entt::registry& registry);

    void update() override;

private:
    void updatePlayerControl();

    void updatePlayerInteract();

    void updateCursor();
    void updateHoveredEntities(const glm::vec2& mouse_world_pos);

    void clearHoveredQueries(entt::registry& registry);

    DIRECTION previous_player_direction{DOWN};
    entt::entity cursor_entity;
    entt::entity player_interacter_entity;

    std::set<entt::entity>* hovered_entities{new std::set<entt::entity>};
    std::set<entt::entity>* last_hovered_entities{new std::set<entt::entity>};
};