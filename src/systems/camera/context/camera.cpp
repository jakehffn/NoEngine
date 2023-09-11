#include "camera.hpp"

Camera::Camera() :

    position{ glm::vec3(0, 0, 0) }, view_matrix{ glm::mat4(1.0f) } {
        this->zoom = 3.0f;

        float left = (constant::SCREEN_WIDTH - (constant::SCREEN_WIDTH/this->zoom)) / 2;
        float right = left + float(constant::SCREEN_WIDTH/this->zoom);
        float top = (constant::SCREEN_HEIGHT - (constant::SCREEN_HEIGHT/this->zoom)) / 2;
        float bottom = top + float(constant::SCREEN_HEIGHT/this->zoom);

        this->projection_matrix = glm::ortho(left, right, 
            bottom, top, -100.0f, 100.0f);

        this->position = glm::vec3(0, 0, 0);
        this->update();

        this->camera_dimensions = glm::vec2(right - left, bottom - top);
}

glm::mat4& Camera::getViewMatrix() {
    return this->view_matrix;
}

glm::mat4& Camera::getProjectionMatrix() {
    return this->projection_matrix;
}

float& Camera::getZoom() {
    return this->zoom;
}

glm::vec2& Camera::getDimensions() {
    return this->camera_dimensions;
}

glm::vec3& Camera::getPosition() {
    return this->position;
}

void Camera::update() {
    float x_offset = constant::SCREEN_WIDTH/2;
    float y_offset = constant::SCREEN_HEIGHT/2;

    glm::vec3 offset(x_offset, y_offset, 0);

    glm::vec3 v_position = this->position - offset;

	this->view_matrix = glm::lookAt(
		v_position,          
		v_position + glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0)
	);
}

void Camera::setPosition(glm::vec3 position) {
    this->position = position;
}

glm::vec2 Camera::pixelToScreenCoords(glm::vec2 screen_coords) {
    return (screen_coords / this->getZoom()) - (this->getDimensions() / 2.0f);
}

