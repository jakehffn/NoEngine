#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "system.h"

#include "camera.h"

#include "camera_controller.h"
#include "spacial.h"

class CameraSystem : public System {

public:
    CameraSystem(entt::registry& registry);

    void update() override;

private:
    void updateCameraToController();

    entt::observer cameraControllerObserver;
};