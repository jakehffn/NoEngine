#pragma once 

#include "tileson.hpp"

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
#include "SpriteShader.h"
#include "TileShader.h"

#include "entities.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);

    void update(entt::registry& registry, Clock clock);
    void systemState() override;

    void updateTiles(std::vector<glm::vec3> tiles);

private:
    void showEntities(entt::registry& registry, Clock clock);
    void updateCamera(entt::registry& registry);

    void renderText(Text text, Spacial spacial);
    void renderSprite(Model model, Sprite sprite, bool guiElement=false);
    
    void renderTiles(Clock clock);

    void updateModels(entt::registry& registry);
    void updateModel(Model& model, Spacial spacial);
    void updateAnimation(Animation& animation, Sprite& sprite, Clock clock);

    void initTextMap();

    ShaderProgram* spriteShader;
    ShaderProgram* tileShader;

    Sprite textSprite;
    Sprite tileSheet;

    Animation tileAnimation;

    std::vector<glm::vec3> tiles{glm::vec3{0, 0, 0}};

    Camera camera;
    Camera guiCamera;

    GLuint quadVAO;
    GLuint tileVBO;

    entt::observer spacialObserver;

    std::unordered_map<char, glm::vec2> textMap;
};