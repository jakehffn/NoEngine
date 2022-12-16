#pragma once 

#include "tileson.hpp"

#include <entt\entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Texture.h"
#include "Spacial.h"
#include "Model.h"
#include "CameraController.h"
#include "Animation.h"
#include "Text.h"
#include "ToRender.h"

#include "Camera.h"
#include "Clock.h"
#include "TextureManager.h"

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

    void update() override;

    void updateTiles();

private:
    void showEntities(entt::registry& registry);
    void updateCamera(entt::registry& registry);

    void renderText(Text text, Spacial spacial);
    void renderSprite(Model model, Texture sprite, bool guiElement=false);
    
    void renderTiles(Clock clock);

    void updateModels(entt::registry& registry);
    void updateModel(Model& model, Spacial spacial);

    void updateTextures();

    void initTextMap();

    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* spriteShader;
    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* tileShader;
    ShaderProgram<>* screenShader;

    Texture textSprite;
    Texture tileSheet;

    Animation tileAnimation;

    std::vector<glm::vec3> tiles; // Tile data for vbo

    Camera camera;
    Camera guiCamera;

    GLuint quadVAO;
    GLuint tileVBO;

    GLuint FBO;
    GLuint renderTexture;

    entt::observer spacialObserver;
    entt::observer tileObserver;
    entt::observer textureObserver;

    std::unordered_map<char, glm::vec2> textMap;
};