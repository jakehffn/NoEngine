#include "PlayerCameraController.h"

PlayerCameraController::PlayerCameraController(PlayerObject* player) : player{ player } {}

void PlayerCameraController::update(glm::vec3& pos) {
    pos = player->getPosition();
}