#include "InputSystem.h"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {}

void InputSystem::update() {

    this->playerControlUpdate();
}

void InputSystem::playerControlUpdate() {

    InputManager& inputManager = this->registry.ctx().at<InputManager&>();

    DIRECTION direction;

    bool upAdded = inputManager.isAdded(SDLK_w);
    bool downAdded = inputManager.isAdded(SDLK_s);
    bool leftAdded = inputManager.isAdded(SDLK_a);
    bool rightAdded = inputManager.isAdded(SDLK_d);

    bool up = inputManager.isActive(SDLK_w);
    bool down = inputManager.isActive(SDLK_s);
    bool left = inputManager.isActive(SDLK_a);
    bool right = inputManager.isActive(SDLK_d);

    bool anyRemoved = inputManager.isRemoved(SDLK_w) || inputManager.isRemoved(SDLK_s) ||
                      inputManager.isRemoved(SDLK_a) || inputManager.isRemoved(SDLK_d);

    if (!(up || down || left || right)) {

        if (anyRemoved) {

            auto playerControlledEntities = registry.view<PlayerControl, Velocity>();

            for (auto entity : playerControlledEntities) {

                this->registry.remove<Velocity>(entity);
            }  
        }

        return;
    }

    // Prioritize new inputs
    if (upAdded && !down) {
        direction = UP;
    } else if (downAdded && !up) {
        direction = DOWN;
    } else if (leftAdded && !right) {
        direction = LEFT;
    } else if (rightAdded && !left) {
        direction = RIGHT;
    // Else, if no new inputs, prioritize the previous direction
    } else if (this->previousPlayerDirection == UP && up && !down) {
        direction = UP;
    } else if (this->previousPlayerDirection == DOWN && down && !up) {
        direction = DOWN;
    } else if (this->previousPlayerDirection == LEFT && left && !right) {
        direction = LEFT;
    } else if (this->previousPlayerDirection == RIGHT && right && !left) {
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

    glm::vec3 velocityDirection = glm::vec3();

    switch(direction) {
        case(UP):
            velocityDirection = glm::vec3(0,-1,0);
            break;
        case(DOWN):
            velocityDirection = glm::vec3(0,1,0);
            break;
        case(LEFT):
            velocityDirection = glm::vec3(-1,0,0);
            break;
        case(RIGHT):
            velocityDirection = glm::vec3(1,0,0);
            break;
        default:

            auto playerControlledEntities = registry.view<PlayerControl, Velocity>();
            
            for (auto entity : playerControlledEntities) {

                this->registry.remove<Velocity>(entity);
            }  

            return;
    }

    auto playerControlledEntities = registry.view<PlayerControl,Spacial>();

    for (auto entity : playerControlledEntities) {

        auto& playerControl = playerControlledEntities.get<PlayerControl>(entity);
        
        this->registry.emplace_or_replace<Velocity>(entity, velocityDirection, playerControl.pixelsPerMillisecond);
        this->registry.patch<Spacial>(entity, [direction](auto& spacial) {
            spacial.direction = direction;
        });
    }

    this->previousPlayerDirection = direction;
}