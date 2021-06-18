#include "PlayerObject.h"

PlayerObject::PlayerObject(glm::vec3 pos, Input* input) :
    GameObject("./src/assets/sprites/BagHead.png", pos), input{ input } {}

void PlayerObject::logic() {

}