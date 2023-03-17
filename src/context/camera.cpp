#include "camera.hpp"

Camera::Camera() :

    position{ glm::vec3(0, 0, 0) }, view_matrix{ glm::mat4(1.0f) } {

        this->zoom = 3.0f;

        // float left = 0;
        // float right = render_c::SCREEN_WIDTH;
        // float top = 0;
        // float bottom = render_c::SCREEN_HEIGHT;

        float left = (render_c::SCREEN_WIDTH - (render_c::SCREEN_WIDTH/this->zoom)) / 2;
        float right = left + float(render_c::SCREEN_WIDTH/this->zoom);
        float top = (render_c::SCREEN_HEIGHT - (render_c::SCREEN_HEIGHT/this->zoom)) / 2;
        float bottom = top + float(render_c::SCREEN_HEIGHT/this->zoom);

        this->projection_matrix = glm::ortho(left, right, 
            bottom, top, -100.0f, 100.0f);

        this->position = glm::vec3(0, 0, 0);
        this->update();

        this->camera_dimensions = glm::vec2(right - left, bottom - top);
}

glm::mat4 Camera::getViewMatrix() const {
    return this->view_matrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return this->projection_matrix;
}

float Camera::getZoom() const {
    return this->zoom;
}

glm::vec2 Camera::getCameraDim() const {
    return this->camera_dimensions;
}

glm::vec2 Camera::getPosition() const {
    return this->position;
}

void Camera::update() {

    // printf("x: %f, y: %f, z: %f ", this->position.x, this->position.y, this->position.z);

    float x_offset = render_c::SCREEN_WIDTH/2;
    float y_offset = render_c::SCREEN_HEIGHT/2;

    glm::vec3 offset(x_offset, y_offset, 0);

    glm::vec3 v_position = this->position - offset;

	this->view_matrix = glm::lookAt(
		v_position,          
		v_position + glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0)
	);
}

void Camera::setPosition(glm::vec3 position) {
    // printf("Camera Pos - x: %f, y: %f\r", position.x, position.y);
    this->position = position;
}

