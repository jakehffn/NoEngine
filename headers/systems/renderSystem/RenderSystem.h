#pragma once 

#include <entt\entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Sprite.h"
#include "Spacial.h"
#include "Model.h"
#include "CameraController.h"

#include "Camera.h"

#include "consts.h"

#include "ShaderProgram.h"
#include "BasicShader.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);

    void update(entt::registry& registry);
    void systemState() override;

private:
    void showEntities(entt::registry& registry);
    void updateCamera(entt::registry& registry);

    void renderSprite(Sprite sprite, Model model);
    void updateModel(Model& model, Sprite sprite, Spacial spacial);

    ShaderProgram* shaderProgram;
    Camera camera;
};