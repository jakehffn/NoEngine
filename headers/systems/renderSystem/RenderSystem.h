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
#include "Text.h"

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

    void renderObject(Model model, Sprite sprite, Spacial spacial);
    void renderText(Text text, Model model, Sprite sprite);

    void renderSprite(Sprite sprite);

    void updateModel(Model& model, Sprite sprite, Spacial spacial);
    void updateAnimation(Animation& animation, Sprite& sprite, Clock clock);

    ShaderProgram* shaderProgram;
    Camera camera;
    Camera guiCamera;
};