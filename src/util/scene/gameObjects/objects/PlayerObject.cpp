#include "PlayerObject.h"

PlayerObject::PlayerObject(glm::vec3 position, Input* input, 
        GLuint shaderProgramID, glm::vec3 rotation) : 
        GameObject("./src/assets/sprites/BagHead.png", position, 
            shaderProgramID, rotation), input{ input } {

};

void PlayerObject::logic() {

}