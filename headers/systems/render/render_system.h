#pragma once 

#include <algorithm>

#include <entt\entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "system.h"
#include "texture.h"
#include "spacial.h"
#include "model.h"
#include "camera_controller.h"
#include "animation.h"
#include "text.h"
#include "to_render.h"
#include "tile_set.h"
#include "tile.h"
#include "renderable.h"
#include "collision.h"

#include "camera.h"
#include "clock.h"
#include "texture_atlas.h"
#include "grid.h"

#include "consts.h"

#include "shader_program.h"
#include "sprite_shader.h"
#include "tile_shader.h"
#include "screen_shader.h"
#include "instanced_shader.h"

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

    std::set<entt::entity> renderQuery;
    std::set<entt::entity> lastRenderQuery;
};