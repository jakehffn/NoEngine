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
#include "to_render_tile.hpp"
#include "tile.hpp"
#include "renderable.hpp"
#include "collision.hpp"

#include "renderer.hpp"
#include "camera.hpp"
#include "clock.hpp"
#include "texture_atlas.hpp"
#include "component_grid.hpp"
#include "shader_manager.hpp"

#include "consts.hpp"

#include "debug_timer.hpp"

class RenderSystem : public System {
public:
    RenderSystem(entt::registry& registry);

    void update() override;
    Renderer* getRenderer();

private:
    void cullEntities();
    void sortEntities();

    void updateModels();
    glm::mat4 getModel(const Spacial& spacial, const Texture& texture);
    glm::mat4 getTileModel(const Spacial& spacial);

    void bufferEntityData();

    void render();

    entt::observer spacial_observer;
    entt::observer texture_observer;

    std::set<entt::entity>* render_query{new std::set<entt::entity>};
    std::set<entt::entity>* last_render_query{new std::set<entt::entity>};

    Renderer renderer;
};