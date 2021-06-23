#pragma once 

#include <entt\entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Sprite.h"
#include "Spacial.h"
#include "Model.h"
#include "CameraController.h"
#include "Animation.h"

#include "Camera.h"
#include "Clock.h"

#include "consts.h"

#include "ShaderProgram.h"
#include "BasicShader.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);

    void update(entt::registry& registry, Clock clock);
    void systemState() override;

private:
    void showEntities(entt::registry& registry, Clock clock);
    void updateCamera(entt::registry& registry);

    void renderSprite(Sprite sprite, Model model, glm::vec2 frameData=glm::vec2(0.0f, 1.0f));

    void updateModel(Model& model, Sprite sprite, Spacial spacial);
    void updateAnimation(Animation& animation, Clock clock);

    ShaderProgram* shaderProgram;
    Camera camera;
};