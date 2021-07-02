#include "Camera.h"

#include <stdio.h>

Camera::Camera() :

    position{ glm::vec3(0, 0, 0) }, viewMatrix{ glm::mat4(1.0f) } {

        float zoom = 4.0f;

        // this->projectionMatrix = glm::ortho(0.5f*(render_c::SCREEN_WIDTH - render_c::SCREEN_WIDTH/zoom), 
        // float(render_c::SCREEN_WIDTH/zoom), float(render_c::SCREEN_HEIGHT/zoom),
        // 0.5f*(render_c::SCREEN_HEIGHT - render_c::SCREEN_HEIGHT/zoom), -100.0f, 100.0f);

        float left = (render_c::SCREEN_WIDTH - (render_c::SCREEN_WIDTH/zoom)) / 2;
        float right = left + float(render_c::SCREEN_WIDTH/zoom);
        float top = (render_c::SCREEN_HEIGHT - (render_c::SCREEN_HEIGHT/zoom)) / 2;
        float bottom = top + float(render_c::SCREEN_HEIGHT/zoom);

        this->projectionMatrix = glm::ortho(left, right, 
            bottom, top, -100.0f, 100.0f);
}

glm::mat4 Camera::getViewMatrix() {
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}

void Camera::update() {

    // printf("x: %f, y: %f, z: %f ", this->position.x, this->position.y, this->position.z);

	this->viewMatrix = glm::lookAt(
		position,          
		position + glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0)
	);
}

void Camera::setPosition(glm::vec3 position) {
    this->position = position;
}

