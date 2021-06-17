#pragma once

#include <vector>

#include "Sprite.h"
#include "Map.h"
#include "ObjectInstance.h"
#include "GameObject.h"
#include "Camera.h"
#include "Clock.h"
#include "ShaderProgram.h"
#include "BasicShader.h"
#include "UserCameraController.h"

class Scene {
public:
    Scene(SDL_Window* window, Clock* clock, Input* input, CameraController* cameraController);

    // Returns ID for shader program (currently just vector pos)
    int addShaderProgram(ShaderProgram* shaderProgram);
    
    void loadMapObjects(Map map);
    ObjectInstance& getGameObject(int gameObjectID);

    // Returns the position of added camera controller
    int addCameraController(CameraController* cameraController);
    void setCameraController(int pos);
    void nextCameraController();

    void render();

private:

    // Returns ID for obj (currenlty just vector pos)
    int addSprite(const char* spritePath);
    // Returns ID for instance (currently just vector pos)
    int addObjectInstance(GameObject gameObject);

    void renderInstance(ObjectInstance instance);

    SDL_Window* window;
    Clock* clock;
    Input* input;

    Camera camera;

    std::vector<CameraController*> cameraControllers;
    int cameraControllerPosition;

    std::vector<ShaderProgram*> shaderPrograms;
    std::vector<Sprite> sprites;
    std::vector<ObjectInstance> objectInstances;
};