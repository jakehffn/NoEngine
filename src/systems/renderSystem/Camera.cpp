#include "Camera.h"

#include <stdio.h>

Camera::Camera() :

    position{ glm::vec3(0, 0, 0) }, viewMatrix{ glm::mat4(1.0f) } {

    this->projectionMatrix = glm::ortho(0.0f, float(render_consts::SCREEN_WIDTH), float(render_consts::SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
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

