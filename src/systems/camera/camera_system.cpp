#include "camera_system.hpp"

CameraSystem::CameraSystem(entt::registry& registry) : System(registry) {}

void CameraSystem::update() {
    this->updateCameraToController();
}

void CameraSystem::updateCameraToController() {

    using namespace entt::literals;
    Camera& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    Clock& clock = this->registry.ctx().at<Clock&>();

    auto controller_entities = this->registry.view<CameraController, Spacial>();
    auto entity = controller_entities.front();

    auto [cameraController, spacial] = controller_entities.get<CameraController, Spacial>(entity);
    
    glm::vec3 lookahead;
    if (this->registry.all_of<Velocity>(entity)) {
        auto velocity = this->registry.get<Velocity>(entity);
        lookahead = velocity.components;
    } else {
        switch(spacial.direction) {
            case UP:
                lookahead = glm::vec3(0, -1, 0);
                break;
            case DOWN:
                lookahead = glm::vec3(0, 1, 0);
                break;
            case LEFT:
                lookahead = glm::vec3(-1, 0, 0);
                break;
            case RIGHT:
                lookahead = glm::vec3(1, 0, 0);
                break;
            default:
                break;
        }
        float lookahead_distance{35.0f};
        lookahead *= lookahead_distance;
    }
    // Target camera relative to the center of the sprite
    glm::vec3 target(spacial.pos + (glm::vec3(spacial.dim, 0) * spacial.scale / 2.0f) + lookahead);
    
    float speed{1.5f};
    float normalized_speed{std::clamp(static_cast<float>(clock.getDeltaTime() / 1000.0)*speed, 0.0f, 1.0f)};
    
    camera.setPosition(glm::mix(camera.getPosition(), target, normalized_speed));

    camera.update();
}