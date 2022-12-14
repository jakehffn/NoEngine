#pragma once

#include <entt/entt.hpp>

#include "Velocity.h"
#include "PlayerControl.h"
#include "Spacial.h"

#include "System.h"

#include "InputManager.h"


class InputSystem : public System {
public:
    InputSystem(entt::registry& registry);

    void update() override;

private:
    void playerControlUpdate();
    DIRECTION previousDirection = DOWN;
};