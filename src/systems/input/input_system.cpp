#include "input_system.hpp"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {
    MapLoader& map_loader = this->registry.ctx().at<MapLoader&>();
    map_loader.connectAfterLoad<&InputSystem::clearHoveredQueries, InputSystem*>(this);

    this->cursor_entity = this->registry.create();
    this->registry.emplace<LoadPrefab>(this->cursor_entity, "CustomCursor");
    
    this->player_interacter_entity = this->registry.create();
    this->registry.emplace<Interacter>(player_interacter_entity);
    this->registry.emplace<Spacial>(player_interacter_entity);
    std::vector<glm::vec4> bounding_boxes;
    bounding_boxes.emplace_back();
    auto& collision = this->registry.emplace<Collision>(player_interacter_entity, bounding_boxes);
    this->registry.emplace<Persistent>(this->player_interacter_entity);
}

void InputSystem::update() {
    DEBUG_TIMER(_, "InputSystem::update");
    this->updatePlayerControl();
    this->updatePlayerInteract();
    this->updateCursor();
}

void InputSystem::updatePlayerControl() {
    Input& input_manager = this->registry.ctx().at<Input&>();

    DIRECTION direction;

    bool up_added = input_manager.isAdded(SDLK_w);
    bool down_added = input_manager.isAdded(SDLK_s);
    bool left_added = input_manager.isAdded(SDLK_a);
    bool right_added = input_manager.isAdded(SDLK_d);

    bool up = input_manager.isActive(SDLK_w);
    bool down = input_manager.isActive(SDLK_s);
    bool left = input_manager.isActive(SDLK_a);
    bool right = input_manager.isActive(SDLK_d);

    bool any_removed = input_manager.isRemoved(SDLK_w) || input_manager.isRemoved(SDLK_s) ||
                      input_manager.isRemoved(SDLK_a) || input_manager.isRemoved(SDLK_d);

    if (!(up || down || left || right)) {
        if (any_removed) {
            auto player_controlled_entities = registry.view<PlayerControl, Velocity>();

            for (auto entity : player_controlled_entities) {
                this->registry.remove<Velocity>(entity);
            }  
        }
        return;
    }

    // Prioritize new inputs
    if (up_added && !down) {
        direction = UP;
    } else if (down_added && !up) {
        direction = DOWN;
    } else if (left_added && !right) {
        direction = LEFT;
    } else if (right_added && !left) {
        direction = RIGHT;
    // Else, if no new inputs, prioritize the previous direction
    } else if (this->previous_player_direction == UP && up && !down) {
        direction = UP;
    } else if (this->previous_player_direction == DOWN && down && !up) {
        direction = DOWN;
    } else if (this->previous_player_direction == LEFT && left && !right) {
        direction = LEFT;
    } else if (this->previous_player_direction == RIGHT && right && !left) {
        direction = RIGHT;
    // Else, if no previous direction, choose a remaining valid direction being pressed
    } else if (up && !down) {
        direction = UP;
    } else if (down && !up) {
        direction = DOWN;
    } else if (left && !right) {
        direction = LEFT;
    } else if (right && !left) {
        direction = RIGHT;
    }

    glm::vec3 velocity_direction = glm::vec3();

    switch(direction) {
        case(UP):
            velocity_direction = glm::vec3(0,-1,0);
            break;
        case(DOWN):
            velocity_direction = glm::vec3(0,1,0);
            break;
        case(LEFT):
            velocity_direction = glm::vec3(-1,0,0);
            break;
        case(RIGHT):
            velocity_direction = glm::vec3(1,0,0);
            break;
        default:
            auto player_controlled_entities = registry.view<PlayerControl, Velocity>();
            
            for (auto entity : player_controlled_entities) {
                this->registry.remove<Velocity>(entity);
            }  

            return;
    }

    auto player_controlled_entities = registry.view<PlayerControl,Spacial>();

    for (auto entity : player_controlled_entities) {
        auto& playerControl = player_controlled_entities.get<PlayerControl>(entity);
        
        this->registry.emplace_or_replace<Velocity>(entity, velocity_direction * playerControl.pixels_per_second);
        this->registry.patch<Spacial>(entity, [direction](auto& spacial) {
            spacial.direction = direction;
        });
    }

    this->previous_player_direction = direction;
}
#include <iostream>
void InputSystem::updatePlayerInteract() {
    Input& input_manager = this->registry.ctx().at<Input&>();

    auto collision = this->registry.get<Collision>(this->player_interacter_entity);

    if (input_manager.isAdded(SDLK_SPACE)) {
        for (auto entity : collision.collisions) {
            if (this->registry.all_of<Interactable>(entity)) {
                this->registry.get<Interactable>(entity).action(this->registry, entity);
                break;
            }
        }
    }

    // Move Player interacter to where the player is
    this->registry.view<PlayerControl, Spacial, Collision>().each([this](auto entity, auto& player_control, auto& player_spacial, auto& player_collision) {
        this->registry.patch<Collision>(this->player_interacter_entity, [player_spacial, player_collision](auto& collision) {
            const float main_axis{30.0f};
            const float cross_axis{20.0f};
            const glm::vec4 primary_player_bb{player_collision.bounding_boxes[0]};

            switch (player_spacial.direction) {
                case UP:
                    collision.bounding_boxes[0] = glm::vec4(
                        cross_axis, 
                        main_axis, 
                        primary_player_bb.z - ((cross_axis - primary_player_bb.x) / 2), 
                        primary_player_bb.w - main_axis - 1
                    );
                    break;
                case DOWN:
                    collision.bounding_boxes[0] = glm::vec4(
                        cross_axis, 
                        main_axis, 
                        primary_player_bb.z - ((cross_axis - primary_player_bb.x) / 2),
                        primary_player_bb.w + primary_player_bb.y + 1
                    );
                    break;
                case LEFT:
                    collision.bounding_boxes[0] = glm::vec4(
                        main_axis, 
                        cross_axis, 
                        primary_player_bb.z - main_axis - 1, 
                        primary_player_bb.w - ((cross_axis - primary_player_bb.y) / 2)
                    );
                    break;
                case RIGHT:
                    collision.bounding_boxes[0] = glm::vec4(
                        main_axis, 
                        cross_axis, 
                        primary_player_bb.z + primary_player_bb.x + 1, 
                        primary_player_bb.w - ((cross_axis - primary_player_bb.y) / 2)
                    );
                    break;
                default:
                    break;
            }
        });

        // Spacial update goes second so that the collisions are updated for the current collision boxes
        this->registry.patch<Spacial>(this->player_interacter_entity, [player_spacial](auto& spacial) {
            spacial.pos = player_spacial.pos;
        });
    });
}

void InputSystem::updateCursor() {
    Input& input_manager = this->registry.ctx().at<Input&>();
    using namespace entt::literals;
    auto& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);

    const glm::vec2 camera_dimensions = camera.getDimensions();
    const glm::vec2 camera_position = camera.getPosition();
    
    glm::vec3 mouse_position = glm::vec3(
        camera.pixelToScreenCoords({input_manager.getMouseX(), input_manager.getMouseY()}),
        2
    );

    this->registry.patch<Spacial>(this->cursor_entity, [mouse_position](auto& spacial) {
        spacial.pos = mouse_position;
    });

    this->registry.clear<LeftClicked>();
    this->registry.clear<RightClicked>();

    if (input_manager.isMouseActive(SDL_BUTTON_LEFT) || 
        input_manager.isMouseAdded(SDL_BUTTON_LEFT) ||
        input_manager.isMouseAdded(SDL_BUTTON_RIGHT)
    ) {
        glm::vec2 mouse_world_pos{mouse_position + camera.getPosition()};
        this->updateHoveredEntities(mouse_world_pos);

        for (auto entity : *this->hovered_entities) {
            
            if (input_manager.isMouseActive(SDL_BUTTON_LEFT)) {
                this->registry.emplace_or_replace<Outline>(entity);
            }
            if (input_manager.isMouseAdded(SDL_BUTTON_LEFT)) {
                this->registry.emplace_or_replace<LeftClicked>(entity);
            }
            if (input_manager.isMouseAdded(SDL_BUTTON_RIGHT)) {
                this->registry.remove<Outline>(entity);
                this->registry.emplace_or_replace<RightClicked>(entity);
            }
        }
    }

    

}

void InputSystem::updateHoveredEntities(const glm::vec2& mouse_world_pos) {
    std::swap(this->last_hovered_entities, this->hovered_entities);
    hovered_entities->clear();

    auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable>&>();
    
    // Get everything around where the mouse is
    std::vector<entt::entity> query_result;
    component_grid.query(
        (lightgrid::bounds) {
            static_cast<int>(mouse_world_pos.x - 5), 
            static_cast<int>(mouse_world_pos.y - 5), 
            10, 10
        }, 
        query_result
    );
    
    // Put everything with their texture under the mouse into a set
    for (auto entity : query_result) {
        if (this->registry.all_of<Spacial, Texture>(entity)) {
            auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
            glm::vec3 real_dim = glm::vec3(texture.frame_data->size.x, texture.frame_data->size.y, 1);
            glm::vec3 offset = glm::vec3(texture.frame_data->offset.x, texture.frame_data->offset.y, 0);
            glm::vec3 real_pos = spacial.pos + offset;

            float hitbox_expansion = 1;

            if (mouse_world_pos.x > real_pos.x - hitbox_expansion && 
                mouse_world_pos.x < real_pos.x + real_dim.x + hitbox_expansion &&
                mouse_world_pos.y > real_pos.y - hitbox_expansion && 
                mouse_world_pos.y < real_pos.y + real_dim.y + hitbox_expansion
            ) {
                this->hovered_entities->insert(entity);
            }
        }
    }
    std::vector<entt::entity> diff;
    // Get the entities which were in the new query but not in the last query
    std::set_difference(this->hovered_entities->begin(), this->hovered_entities->end(),
        this->last_hovered_entities->begin(), this->last_hovered_entities->end(),
        std::back_inserter(diff)
    );

    for (auto entity : diff) {
        this->registry.emplace<Hovered>(entity);
    }
    diff.clear();

    std::set_difference(this->last_hovered_entities->begin(), this->last_hovered_entities->end(),
        this->hovered_entities->begin(), this->hovered_entities->end(),
        std::back_inserter(diff)
    );

    for (auto entity : diff) {
        this->registry.remove<Hovered>(entity);
    }
}

void InputSystem::clearHoveredQueries(entt::registry& registry) {
    this->hovered_entities->clear();
    this->last_hovered_entities->clear();
    this->registry.clear<Hovered>();
}