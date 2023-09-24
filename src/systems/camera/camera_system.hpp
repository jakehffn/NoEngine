#pragma once

#include <algorithm>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.hpp"

#include "camera.hpp"
#include "clock.hpp"

#include "camera_controller.hpp"
#include "spacial.hpp"
#include "velocity.hpp"

class CameraSystem : public System {

public:
    CameraSystem(entt::registry& registry);

    void update() override;

private:
    void updateCameraToController();
    void updateCameraToControllerSmooth();

    entt::observer camera_controller_observer;
};