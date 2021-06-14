#pragma once

#include <vector>

#include "Sprite.h"
#include "Instance.h"
#include "Camera.h"
#include "Clock.h"
#include "ShaderProgram.h"
#include "UserCameraController.h"

class Scene {
public:
    Scene(SDL_Window* window, Clock* clock, Input* input, CameraController* cameraController);

    // Returns ID for shader program (currently just vector pos)
    int addShaderProgram(ShaderProgram* shaderProgram);
    // Returns ID for obj (currenlty just vector pos)
    int addSprite(char* spritePath);
    // Returns ID for instance (currently just vector pos)
    int addInstance(int spriteID, glm::vec3 position, 
        glm::vec3 scale=glm::vec3(1, 1, 1), glm::vec3 rotation=glm::vec3(0, 0, 0));
    Instance& getInstance(int instanceID);

    // Returns the position of added camera controller
    int addCameraController(CameraController* cameraController);
    void setCameraController(int pos);
    void nextCameraController();

    void render();

private:
    void renderInstance(Instance instance);
    void renderEnvironment();

    SDL_Window* window;
    Clock* clock;
    Input* input;

    Camera camera;

    std::vector<CameraController*> cameraControllers;
    int cameraControllerPosition;

    std::vector<ShaderProgram*> shaderPrograms;
    std::vector<Sprite> sprites;
    std::vector<Instance> instances;
};