#include "BackgroundObject.h"

BackgroundObject::BackgroundObject(Clock* clock, GLuint shaderProgramID) :
    SpriteObject("./src/assets/sprites/Screenshot (76).png", glm::vec3(0, 0, -.1)), clock{ clock } {}

void BackgroundObject::logic() {}