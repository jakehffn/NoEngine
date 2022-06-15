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
#include "ToRender.h"

#include "Camera.h"
#include "Clock.h"

#include "consts.h"

#include "ShaderProgram.h"
#include "SpriteShader.h"
#include "TileShader.h"
#include "ScreenShader.h"

#include "entities.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);
    ~RenderSystem();

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

    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* spriteShader;
    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* tileShader;
    ShaderProgram<>* screenShader;

    Sprite textSprite;
    Sprite tileSheet;

    Animation tileAnimation;

    std::vector<glm::vec3> tiles; // Tile data for vbo

    Camera camera;
    Camera guiCamera;

    GLuint quadVAO;
    GLuint tileVBO;

    GLuint FBO;
    GLuint renderTexture;

    entt::observer spacialObserver;

    std::unordered_map<char, glm::vec2> textMap;
};