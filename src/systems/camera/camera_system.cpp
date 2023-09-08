#include "camera_system.hpp"

CameraSystem::CameraSystem(entt::registry& registry) : System(registry) {}

void CameraSystem::update() {
    this->updateCameraToController();
}

void CameraSystem::updateCameraToController() {

    using namespace entt::literals;
    Camera& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);

    auto controller_entities = this->registry.view<CameraController, Spacial>();

    for (auto entity : controller_entities) {

        auto [cameraController, spacial] = controller_entities.get<CameraController, Spacial>(entity);

        float x_offset = spacial.dim.x * spacial.scale.x / 2;
        float y_offset = spacial.dim.y * spacial.scale.y / 2;

        glm::vec3 offset(x_offset, y_offset, 0);
        camera.setPosition(spacial.pos + offset);
    }

    camera.update();
}