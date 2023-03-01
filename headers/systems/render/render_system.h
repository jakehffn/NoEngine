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

#include "renderer.h"
#include "camera.h"
#include "clock.h"
#include "texture_atlas.h"
#include "grid.h"

#include "consts.h"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);

    void update() override;

private:
    void initTextMap();

    void cullEntities();
    void sortEntities();

    void updateModels();
    glm::mat4 getModel(const Spacial& spacial, const Texture& texture);
    glm::mat4 getTileModel(const Spacial& spacial);

    void bufferEntityData();
    void bufferTileData();

    void render();

    entt::observer spacialObserver;
    entt::observer textureObserver;

    std::unordered_map<char, glm::vec2> textMap;

    std::set<entt::entity> renderQuery;
    std::set<entt::entity> lastRenderQuery;

    Renderer renderer;
};