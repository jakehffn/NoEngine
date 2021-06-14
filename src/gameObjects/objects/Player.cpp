#include "player.h"

Player::Player(Scene* scene, glm::vec3 pos) :
    GameObject(scene, "./src/assets/sprites/BagHead.png") {

    int spriteID = this->scene->addSprite(this->spritePath);
    int instanceID = this->scene->addInstance(spriteID, glm::vec3(200, 200, 0));
    this->instance = &(this->scene->getInstance(instanceID));
}

void Player::setPos(glm::vec3 pos) {
    this->instance->setPosition(pos);
}

void Player::move(glm::vec3 movement) {
    this->instance->addPosition(movement);
}