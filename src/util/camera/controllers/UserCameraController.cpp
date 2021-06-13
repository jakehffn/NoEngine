#include "UserCameraController.h"

UserCameraController::UserCameraController(SDL_Window* window, Clock* clock, Input* input) :
    clock{ clock }, window{ window }, input{ input } {

    // SDL_ShowCursor(SDL_DISABLE);

    input->update();
}

void UserCameraController::update(glm::vec3& pos) {

    float deltaTime = (float)clock->getDeltaTime();

    glm::vec3 forward(0, 1, 0);
    glm::vec3 right(1, 0, 0);

    // Change this later so that moving both direction does give sqrt(2) speed 

    // Move forward
    if (input->isKeyDown(SDLK_w)) {
        pos -= forward * deltaTime * input_consts::KEY_SPEED;
    }
    // Move backward
    if (input->isKeyDown(SDLK_s)) {
        pos += forward * deltaTime * input_consts::KEY_SPEED;
    }
    // Strafe left
    if (input->isKeyDown(SDLK_a)) {
        pos -= right * deltaTime * input_consts::KEY_SPEED;
    }
    // Strafe right
    if (input->isKeyDown(SDLK_d)) {
        pos += right * deltaTime * input_consts::KEY_SPEED;
    }
}