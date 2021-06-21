#pragma once

#include <vector>

#include <entt/entity/registry.hpp>

#include "Sprite.h"
#include "Map.h"

#include "SpriteObject.h"
#include "LogicObject.h"

#include "Camera.h"
#include "Clock.h"
#include "ShaderProgram.h"
#include "BasicShader.h"
#include "UserCameraController.h"

class Scene {
public:
    Scene(SDL_Window* window, Clock* clock, Input* input, CameraController* cameraController);
    
    void loadMapObjects(Map map);

    // Returns the position of added camera controller
    int addCameraController(CameraController* cameraController);
    void setCameraController(int pos);
    void nextCameraController();

    void loop();

private:
    void render();
    void renderInstance(SpriteObject* gameObject);

    void logic();

    SDL_Window* window;
    Clock* clock;
    Input* input;

    Camera camera;
    ShaderProgram* shaderProgram;

    std::vector<CameraController*> cameraControllers;
    int cameraControllerPosition;

    entt::registry registry;
};