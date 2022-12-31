#pragma once 

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
#include "TileSet.h"

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
    void initTextMap();
    void initVAO();
    void initTileVBO(TileSet& tileSet);
    void initScreenFBO();

    void renderTiles();
    void renderEntities();
    void renderText(Text text, Spacial spacial);
    void renderTexture(Model model, Texture sprite, bool guiElement=false);
    
    void updateModels();
    void updateModel(Model& model, Spacial spacial);

    void updateTextures();

    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* spriteShader;
    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* tileShader;
    ShaderProgram<>* screenShader;

    Texture textSprite;

    // Tiledata should maybe be put into a tilesheet component, where
    //  with an entity that has the tilesheet and then the texture for the 
    //  associated tiles and also the animation. The tile system should also be
    //  able to do the tile updates

    GLuint VAO;

    GLuint screenFBO;
    GLuint screenTexture;

    entt::observer spacialObserver;
    entt::observer textureObserver;

    std::unordered_map<char, glm::vec2> textMap;
};