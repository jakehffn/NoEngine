#include "input_system.hpp"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {}

void InputSystem::update() {

    this->playerControlUpdate();
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