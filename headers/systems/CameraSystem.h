#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "System.h"

#include "Camera.h"

#include "CameraController.h"
#include "Spacial.h"

class CameraSystem : public System {

public:
    CameraSystem(entt::registry& registry);

    void update() override;

private:
    void updateCameraToController();

    entt::observer cameraControllerObserver;
};