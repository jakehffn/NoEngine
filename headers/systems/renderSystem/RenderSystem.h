#pragma once 

#include <entt\entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Sprite.h"
#include "Spacial.h"
#include "Model.h"

#include "Camera.h"

#include "ShaderProgram.h"
#include "BasicShader.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);

    void update(entt::registry& registry);
private:
    void renderSprite(Sprite sprite, Model model);
    void updateModel(Model& model, Spacial spacial);

    ShaderProgram* shaderProgram;
    Camera camera;
};