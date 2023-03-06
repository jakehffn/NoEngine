#include "camera.hpp"

// #include <stdio.h>

Camera::Camera() :

    position{ glm::vec3(0, 0, 0) }, viewMatrix{ glm::mat4(1.0f) } {

        this->zoom = 3.0f;

        // float left = 0;
        // float right = render_c::SCREEN_WIDTH;
        // float top = 0;
        // float bottom = render_c::SCREEN_HEIGHT;

        float left = (render_c::SCREEN_WIDTH - (render_c::SCREEN_WIDTH/this->zoom)) / 2;
        float right = left + float(render_c::SCREEN_WIDTH/this->zoom);
        float top = (render_c::SCREEN_HEIGHT - (render_c::SCREEN_HEIGHT/this->zoom)) / 2;
        float bottom = top + float(render_c::SCREEN_HEIGHT/this->zoom);

        this->projectionMatrix = glm::ortho(left, right, 
            bottom, top, -100.0f, 100.0f);

        this->position = glm::vec3(0, 0, 0);
        this->update();

        this->cameraDim = glm::vec2(right - left, bottom - top);
}

glm::mat4 Camera::getViewMatrix() const {
    return this->viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return this->projectionMatrix;
}

float Camera::getZoom() const {
    return this->zoom;
}

glm::vec2 Camera::getCameraDim() const {
    return this->cameraDim;
}

glm::vec2 Camera::getPosition() const {
    return this->position;
}

void Camera::update() {

    // printf("x: %f, y: %f, z: %f ", this->position.x, this->position.y, this->position.z);

    float xOffset = render_c::SCREEN_WIDTH/2;
    float yOffset = render_c::SCREEN_HEIGHT/2;

    glm::vec3 offset(xOffset, yOffset, 0);

    glm::vec3 vPosition = this->position - offset;

	this->viewMatrix = glm::lookAt(
		vPosition,          
		vPosition + glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0)
	);
}

void Camera::setPosition(glm::vec3 position) {
    // printf("Camera Pos - x: %f, y: %f\r", position.x, position.y);
    this->position = position;
}

