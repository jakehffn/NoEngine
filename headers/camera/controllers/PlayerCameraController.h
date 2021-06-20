#pragma once 

#include "CameraController.h"
#include "PlayerObject.h"

class PlayerCameraController : public CameraController {
public:
    PlayerCameraController(PlayerObject* player);

    void update(glm::vec3& pos);

private:
    PlayerObject* player;
};