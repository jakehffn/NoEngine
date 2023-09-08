#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.hpp"

#include "camera.hpp"

#include "camera_controller.hpp"
#include "spacial.hpp"

class CameraSystem : public System {

public:
    CameraSystem(entt::registry& registry);

    void update() override;

private:
    void updateCameraToController();

    entt::observer camera_controller_observer;
};