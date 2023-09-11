#pragma once

#include <vector>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "consts.hpp"

/*
    The Camera class handles generation of the view and projection matricies 
using the position, direction, and the vector pointing to the right which are 
all generated in an instance of a implementation of the CameraController class.

No time delta should be tracked within the Camera class.
*/

class Camera {
public:
    Camera();

    glm::mat4& getViewMatrix();
    glm::mat4& getProjectionMatrix();
    float& getZoom();
    glm::vec2& getCameraDim();
    glm::vec3& getPosition();

    void update();

    void setPosition(glm::vec3 pos);

private:
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::vec3 position; 

    float zoom;
    glm::vec2 camera_dimensions;
};
