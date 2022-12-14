#include "InputSystem.h"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {}

void InputSystem::update() {

    this->playerControlUpdate();
}

void InputSystem::playerControlUpdate() {

    InputManager& inputManager = this->registry.ctx().at<InputManager&>();

    DIRECTION direction;

    bool newUp = inputManager.isAdded(SDLK_w);
    bool newDown = inputManager.isAdded(SDLK_s);
    bool newLeft = inputManager.isAdded(SDLK_a);
    bool newRight = inputManager.isAdded(SDLK_d);

    bool up = inputManager.isActive(SDLK_w);
    bool down = inputManager.isActive(SDLK_s);
    bool left = inputManager.isActive(SDLK_a);
    bool right = inputManager.isActive(SDLK_d);

    if (!(up || down || left || right)) {
        return;
    }

    if (newUp || newDown || newLeft || newRight) {

        if (newUp && !down) {
            direction = UP;
        } else if (newDown && !up) {
            direction = DOWN;
        } else if (newLeft && !right) {
            direction = LEFT;
        } else if (newRight && !left) {
            direction = RIGHT;
        }

    } else {

        if (previousDirection == UP && up && !down) {
            direction = UP;
        } else if (previousDirection == DOWN && down && !up) {
            direction = DOWN;
        } else if (previousDirection == LEFT && left && !right) {
            direction = LEFT;
        } else if (previousDirection == RIGHT && right && !left) {
            direction = RIGHT;
        } else if (up && !down) {
            direction = UP;
        } else if (down && !up) {
            direction = DOWN;
        } else if (left && !right) {
            direction = LEFT;
        } else if (right && !left) {
            direction = RIGHT;
        }
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
    }

    auto playerControlledEntities = registry.view<PlayerControl,Spacial>();

    for (auto entity : playerControlledEntities) {

        auto& playerControl = playerControlledEntities.get<PlayerControl>(entity);
        
        this->registry.emplace_or_replace<Velocity>(entity, velocityDirection, playerControl.pps);
        this->registry.patch<Spacial>(entity, [direction](auto& spacial) {
            spacial.direction = direction;
        });
    }

    this->previousDirection = direction;
}