#include "input_system.hpp"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {}

void InputSystem::update() {
    if (this->cursor == entt::null) {
        const auto& entity = this->registry.create();
        this->registry.emplace<Spacial>(entity, glm::vec3(0, 0, 1), 
            glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(5, 5)
        );
        ResourceLoader::createDefault(registry, entity, "./assets/misc/PinkFlower/placeholder.png");
        this->registry.emplace<Cursor>(entity);
        this->cursor = entity;
    }
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

    this->registry.clear<Outline>();

    using namespace entt::literals;
    auto& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable>&>();

    glm::vec2 camera_dimensions = camera.getCameraDim();
    glm::vec2 camera_position = camera.getPosition();

    int x = camera_position.x - camera_dimensions.x/2 + input_manager.getMouseX() / camera.getZoom();
    int y = camera_position.y - camera_dimensions.y/2 + input_manager.getMouseY() / camera.getZoom();
    
    std::vector<entt::entity> query_result;
    component_grid.query((lightgrid::bounds) {x,y,1,1}, query_result);
    // component_grid.query(x, y, *this->render_query);
    for (auto entity : query_result) {
        this->registry.emplace<Outline>(entity);
    }

    this->registry.patch<Spacial>(this->cursor, [x, y](auto& spacial) {
        spacial.pos = glm::vec3(x, y, 2);
    });
}