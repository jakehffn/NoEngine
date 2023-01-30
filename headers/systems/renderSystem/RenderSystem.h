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
#include "Tile.h"
#include "Renderable.h"

#include "Camera.h"
#include "Clock.h"
#include "TextureAtlas.h"

#include "consts.h"

#include "ShaderProgram.h"
#include "SpriteShader.h"
#include "TileShader.h"
#include "ScreenShader.h"
#include "InstancedShader.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);
    ~RenderSystem();

    void update() override;

private:
    void initTextMap();
    void initVAO();
    void initVBOs();
    void initScreenFBO();

    void fillBufferData();
    void addTileBufferData();
    void addEntityBufferData();

    void render();

    void cullEntities();
    void sortEntities();

    void updateModels();
    glm::mat4 getModel(Spacial spacial, Texture texture);
    glm::mat4 getTileModel(Spacial spacial);

    // void renderTiles();
    // void renderEntities();
    // void renderText(Text text, Spacial spacial);
    // void renderTexture(Model model, Texture sprite, bool guiElement=false);
    

    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* spriteShader;
    ShaderProgram<glm::mat4, glm::mat4, glm::mat4, glm::vec2>* tileShader;
    InstancedShader* instancedShader;
    ShaderProgram<double>* screenShader;

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

    GLuint textureCoordinatesVBO;
    std::vector<glm::vec4> textureCoordinatesBufferData; 

    GLuint modelsVBO;
    std::vector<glm::mat4> modelsBufferData; 
};