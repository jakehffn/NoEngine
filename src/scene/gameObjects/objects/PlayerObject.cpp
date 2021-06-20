#include "PlayerObject.h"

PlayerObject::PlayerObject(glm::vec3 position, Input* input, Clock* clock, 
        GLuint shaderProgramID, glm::vec3 rotation) : 
        SpriteObject("./src/assets/sprites/BagHead.png", position, 
            shaderProgramID, rotation), input{ input }, clock{ clock } {}

void PlayerObject::logic() {

    float deltaTime = (float)this->clock->getDeltaTime();

    glm::vec3 prevPos = this->position;

    glm::vec3 forward(0, 1, 0);
    glm::vec3 right(1, 0, 0);

    // Change this later so that moving both direction does give sqrt(2) speed 

    // Move forward
    if (input->isKeyDown(SDLK_w)) {
        this->position -= forward * deltaTime * input_consts::KEY_SPEED;
    }
    // Move backward
    if (input->isKeyDown(SDLK_s)) {
        this->position += forward * deltaTime * input_consts::KEY_SPEED;
    }
    // Strafe left
    if (input->isKeyDown(SDLK_a)) {
        this->position -= right * deltaTime * input_consts::KEY_SPEED;
    }
    // Strafe right
    if (input->isKeyDown(SDLK_d)) {
        this->position += right * deltaTime * input_consts::KEY_SPEED;
    }

    if (this->position != prevPos) {
        this->needsUpdate = true;
    }

}