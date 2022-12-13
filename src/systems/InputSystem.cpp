#include "InputSystem.h"

InputSystem::InputSystem (entt::registry& registry) : System(registry) {}

void InputSystem::update() {

    this->playerControlUpdate();
}

void InputSystem::playerControlUpdate() {

    if (this->registry.ctx().at<InputManager&>().isActive(SDLK_SPACE)) {

        auto playerControlledEntities = registry.view<PlayerControl>();

        for (auto entity : playerControlledEntities) {

            auto& playerControl = this->registry.get<PlayerControl>(entity);
            
            this->registry.emplace_or_replace<Velocity>(entity, glm::vec3(1,0,0), playerControl.pps);
        }

    }

    
}