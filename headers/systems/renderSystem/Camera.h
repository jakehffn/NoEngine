#pragma once

#include <vector>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "consts.h"

/*
    The Camera class handles generation of the view and projection matricies 
using the position, direction, and the vector pointing to the right which are 
all generated in an instance of a implementation of the CameraController class.

No time delta should be tracked within the Camera class.
*/

class Camera {
public:
    Camera();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    float getZoom() const;
    glm::vec2 getCameraDim() const;
    glm::vec2 getPosition() const;

    void update();

    void setPosition(glm::vec3 pos);

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 position; 

    float zoom;
    glm::vec2 cameraDim;
};
