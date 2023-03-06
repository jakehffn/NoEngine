#pragma once 

#include <algorithm>

#include <entt\entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "system.hpp"
#include "texture.hpp"
#include "spacial.hpp"
#include "model.hpp"
#include "camera_controller.hpp"
#include "animation.hpp"
#include "text.hpp"
#include "to_render.hpp"
#include "tile.hpp"
#include "renderable.hpp"
#include "collision.hpp"

#include "renderer.hpp"
#include "camera.hpp"
#include "clock.hpp"
#include "texture_atlas.hpp"
#include "component_grid.hpp"

#include "consts.hpp"

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

    void bufferEntityData();

    void render();

    entt::observer spacialObserver;
    entt::observer textureObserver;

    std::unordered_map<char, glm::vec2> textMap;

    std::set<entt::entity> renderQuery;
    std::set<entt::entity> lastRenderQuery;

    Renderer renderer;
};