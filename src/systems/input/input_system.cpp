#include "input_system.hpp"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {
    this->cursor_entity = this->registry.create();
    ResourceLoader::create(this->registry, this->cursor_entity, "CustomCursor");
}

void InputSystem::update() {
    DEBUG_TIMER(_, "InputSystem::update");
    this->playerControlUpdate();
    this->selectedUpdate();
}

void InputSystem::playerControlUpdate() {

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
        
        this->registry.emplace_or_replace<Velocity>(entity, velocity_direction, playerControl.pixels_per_millisecond);
        this->registry.patch<Spacial>(entity, [direction](auto& spacial) {
            spacial.direction = direction;
        });
    }

    this->previous_player_direction = direction;
}

#include <iostream>

void InputSystem::selectedUpdate() {
    Input& input_manager = this->registry.ctx().at<Input&>();
    using namespace entt::literals;
    auto& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    glm::vec2 camera_dimensions = camera.getCameraDim();
    glm::vec2 camera_position = camera.getPosition();

    this->registry.clear<LeftClicked>();
    this->registry.clear<RightClicked>();

    if (input_manager.isMouseActive(SDL_BUTTON_LEFT) || 
        input_manager.isMouseAdded(SDL_BUTTON_LEFT) ||
        input_manager.isMouseAdded(SDL_BUTTON_RIGHT)
    ) {

        auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable>&>();

        int x = camera_position.x - camera_dimensions.x/2 + input_manager.getMouseX() / camera.getZoom();
        int y = camera_position.y - camera_dimensions.y/2 + input_manager.getMouseY() / camera.getZoom();
        
        std::vector<entt::entity> query_result;
        component_grid.query((lightgrid::bounds) {x,y,1,1}, query_result);
        // component_grid.query(x, y, *this->render_query);
        for (auto entity : query_result) {
            if (this->registry.all_of<Spacial, Texture>(entity)) {
                auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
                glm::vec3 real_dim = glm::vec3(texture.frame_data->size.x, texture.frame_data->size.y, 1);
                glm::vec3 offset = glm::vec3(texture.frame_data->offset.x, texture.frame_data->offset.y, 0);
                glm::vec3 real_pos = spacial.pos + offset;

                float hitbox_expansion = 0.5;

                if (x > real_pos.x - hitbox_expansion && x < real_pos.x + real_dim.x + hitbox_expansion &&
                    y > real_pos.y - hitbox_expansion && y < real_pos.y + real_dim.y + hitbox_expansion
                ) {
                    if (input_manager.isMouseActive(SDL_BUTTON_LEFT)) {
                        this->registry.emplace_or_replace<Outline>(entity);
                    }
                    if (input_manager.isMouseAdded(SDL_BUTTON_LEFT)) {
                        this->registry.emplace_or_replace<LeftClicked>(entity);
                    }
                    if (input_manager.isMouseAdded(SDL_BUTTON_RIGHT)) {
                        // this->registry.clear<Outline>();
                        this->registry.emplace_or_replace<RightClicked>(entity);
                    }
                }
            }
        }
    }

    glm::vec3 mouse_position = glm::vec3(
        input_manager.getMouseX() / camera.getZoom() - camera_dimensions.x/2, 
        input_manager.getMouseY() / camera.getZoom() - camera_dimensions.y/2, 
        2
    );

    this->registry.patch<Spacial>(this->cursor_entity, [mouse_position](auto& spacial) {
        spacial.pos = mouse_position;
    });

}