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

    float x_offset = spacial.dimensions.x * spacial.scale.x / 2;
    float y_offset = spacial.dimensions.y * spacial.scale.y / 2;

    glm::vec3 offset(x_offset, y_offset, 0);
    const glm::vec3 normalized_position = glm::vec3(glm::ivec3(spacial.position*camera.getZoom()) + glm::ivec3(0.5, 0.5, 0))/camera.getZoom();
    camera.setPosition(normalized_position + offset);

    camera.update();
}

void CameraSystem::updateCameraToControllerSmooth() {

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
        float lookahead_distance{10.0f};
        lookahead *= lookahead_distance;
    }
    // Target camera relative to the center of the sprite
    glm::vec3 target(spacial.position + (glm::vec3(spacial.dimensions, 0) * spacial.scale / 2.0f) + lookahead);
    
    float speed{1.5f};
    float normalized_speed{std::clamp(static_cast<float>(clock.getDeltaTime() / 1000.0)*speed, 0.0f, 1.0f)};
    // normalized_speed = (normalized_speed > 0.01) ? normalized_speed : 0;
    
    glm::vec3 camera_position = glm::vec3(glm::mix(camera.getPosition(), target, normalized_speed));
    // Help prevent texture bleeding by rounding to full pixels
    // The models are rounded and then half a pixel is added
    // camera_position = glm::vec3(glm::ivec3(camera_position * camera.getZoom())) / camera.getZoom();
    camera.setPosition(camera_position);

    camera.update();
}