#include "render_system.hpp"

RenderSystem::RenderSystem(entt::registry& registry) : System(registry),
    spacial_observer{entt::observer(registry, entt::collector.update<Spacial>().where<Texture>())},
    spacial_tile_observer{entt::observer(registry, entt::collector.update<Spacial>().where<Tile>())},
    texture_observer{entt::observer(registry, entt::collector.update<Texture>().where<Spacial>())} {
        this->registry.on_construct<Texture>().connect<&RenderSystem::initModel>();
        this->registry.on_construct<Tile>().connect<&RenderSystem::initTileModel>();
        auto& shader_manager = this->registry.ctx().at<ShaderManager&>();

        MapLoader& map_loader = this->registry.ctx().at<MapLoader&>();
        map_loader.connectAfterLoad<&RenderSystem::clearRenderQueries>(this);
    }

void RenderSystem::update() {
    DEBUG_TIMER(_, "RenderSystem::update");
    this->renderer.clear();
    this->cullEntities();
    this->sortEntities();
    this->updateModels();
    this->render();
}

Renderer* RenderSystem::getRenderer() {
    return &this->renderer;
}

void RenderSystem::cullEntities() {
    DEBUG_TIMER(_, "RenderSystem::cullEntities");

    using namespace entt::literals;
    auto& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable>&>();

    glm::vec2 camera_dimensions = camera.getDimensions();
    glm::vec2 camera_position = camera.getPosition();

    int x = camera_position.x - camera_dimensions.x/2;
    int y = camera_position.y - camera_dimensions.y/2;
    int w = camera_dimensions.x + 16;
    int h = camera_dimensions.y + 16;
  
    component_grid.query((lightgrid::bounds) {x,y,w,h}, *this->render_query);

    std::vector<entt::entity> diff;
    {
        DEBUG_TIMER(set_difference_timer, "RenderSystem::cullEntities - set difference");
        // Get the entities which were in the new query but not in the last query
        std::set_difference(this->render_query->begin(), this->render_query->end(),
            this->last_render_query->begin(), this->last_render_query->end(),
            std::back_inserter(diff)
        );
    }

    for (auto entity : diff) {
        if (this->registry.all_of<Tile>(entity)) {
            this->registry.emplace<ToRenderTile>(entity);
        } else {
            this->registry.emplace<ToRender>(entity);
        }
    }
    diff.clear();

    // auto total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
    // Get the entities which were in the last query but not in the new query
    std::set_difference(this->last_render_query->begin(), this->last_render_query->end(),
        this->render_query->begin(), this->render_query->end(),
        std::back_inserter(diff)
    );

    for (auto entity : diff) {
        this->registry.remove<ToRender>(entity);
        this->registry.remove<ToRenderTile>(entity);
    }

    std::swap(this->last_render_query, this->render_query);
    render_query->clear();
}

void RenderSystem::sortEntities() {
    DEBUG_TIMER(_, "RenderSystem::sortEntities");
    // Sort sprites by Spacial y-pos before rendering
    // Tiles don't need to be sorted
    this->registry.sort<ToRender>([this](const entt::entity lhs, const entt::entity rhs) {
        auto lhSpacial = this->registry.get<Spacial>(lhs);
        auto rhSpacial = this->registry.get<Spacial>(rhs);
        
        return (lhSpacial.pos.y + lhSpacial.dim.y) * ((lhSpacial.pos.z + 1)*10) < (rhSpacial.pos.y + rhSpacial.dim.y) * ((rhSpacial.pos.z + 1)*10);

    }, entt::insertion_sort {}); // Insertion sort is much faster as the spacials will generally be "mostly sorted"
}

void RenderSystem::clearRenderQueries(entt::registry& registry) {
    this->render_query->clear();
    this->last_render_query->clear();
    this->registry.clear<ToRender>();
    this->registry.clear<ToRenderTile>();
}

void RenderSystem::updateModels() {
    DEBUG_TIMER(_, "RenderSystem::updateModels");
    using namespace entt::literals;
    Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
    // TDOD: Consider only updating models of entities which need rendering

    {
        DEBUG_TIMER(spacial_observer_timer, "Spacial Observer");
        // Update the models of all the entities whose spacials have been changed
        this->spacial_observer.each([this, &camera](entt::entity entity){
            auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
            this->registry.emplace_or_replace<Model>(entity, RenderSystem::getModel(spacial, texture, camera.getZoom()));
        });
    }
    {
        DEBUG_TIMER(spacial_tile_observer_timer, "Spacial Tile Observer");
        this->spacial_tile_observer.each([this, &camera](entt::entity entity) {
            auto spacial = this->registry.get<Spacial>(entity);
            this->registry.emplace_or_replace<Model>(entity, RenderSystem::getTileModel(spacial));
        });
    }
    {
        DEBUG_TIMER(texture_observer_timer, "Texture Observer");
        // ...and same for models with updated textures
        // TODO: Consider ways of avoiding overlap between these two groups
        this->texture_observer.each([this, &camera](entt::entity entity){
            auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
            this->registry.emplace_or_replace<Model>(entity, RenderSystem::getModel(spacial, texture, camera.getZoom()));
        });
    }
}

glm::mat4 RenderSystem::getModel(const Spacial& spacial, const Texture& texture, const float camera_zoom) {
    // The model does not represent the physical location exactly, but the rendered location
    //  Information from the texture is needed so that the sprite can be placed correctly
    glm::mat4 rotate = glm::mat4(1.0f);
    
    rotate = glm::rotate(rotate, spacial.rot.x, glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, spacial.rot.y, glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, spacial.rot.z, glm::vec3(0, 0, 1));

    glm::vec3 scale_vector = glm::vec3(spacial.scale.x, spacial.scale.y, spacial.scale.z);
    glm::vec3 dimensions_vector = glm::vec3(texture.frame_data->size.x, texture.frame_data->size.y, 1);
    glm::vec3 size_vector = scale_vector*dimensions_vector;

    glm::mat4 scale = glm::scale(glm::mat4(1), scale_vector*dimensions_vector);

    glm::vec3 offset = glm::vec3(texture.frame_data->offset.x, texture.frame_data->offset.y, 0);

    // Help prevent texture bleeding by rounding to full pixels
    // The camera rounds to a full pixel, while this rounds to a pixel plus half a pixel
    glm::vec3 normalized_position = glm::vec3(glm::ivec3(spacial.pos*camera_zoom) + glm::ivec3(0.5, 0.5, 0))/camera_zoom;
    glm::mat4 translate = glm::translate(glm::mat4(1), normalized_position + (offset*scale_vector));

    glm::mat4 center = glm::translate(glm::mat4(1), -1.0f * (size_vector/2.0f + offset/2.0f));
    glm::mat4 uncenter = glm::translate(glm::mat4(1), (size_vector/2.0f + offset/2.0f));

    // Order matters
    return (translate * uncenter * rotate * center * scale);
}

glm::mat4 RenderSystem::getTileModel(const Spacial& spacial) {
    AtlasData tile_frame_data = {
        {0, 0},
        {16, 16},
        {0, 0}
    };
    return RenderSystem::getModel(spacial, {"", &tile_frame_data}, 1);
}

void RenderSystem::initModel(entt::registry& registry, entt::entity entity) {
    using namespace entt::literals;
    Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
    if (registry.all_of<Spacial>(entity)) {
        auto [spacial, texture] = registry.get<Spacial, Texture>(entity);
        registry.emplace_or_replace<Model>(entity, RenderSystem::getModel(spacial, texture, camera.getZoom()));
    }
}

void RenderSystem::initTileModel(entt::registry& registry, entt::entity entity) {
    if (registry.all_of<Spacial>(entity)) {
        auto spacial = registry.get<Spacial>(entity);
        registry.emplace_or_replace<Model>(entity, RenderSystem::getTileModel(spacial));
    } 
}

void RenderSystem::render() {
    DEBUG_TIMER(_, "RenderSystem::render");

    auto& shader_manager = this->registry.ctx().at<ShaderManager&>();

    using namespace entt::literals;
    Camera& camera = registry.ctx().at<Camera&>("world_camera"_hs);
    shader_manager["instanced"]->setUniform("P", &camera.getProjectionMatrix()[0][0]);
    shader_manager["instanced"]->setUniform("V", &camera.getViewMatrix()[0][0]);
    shader_manager["instanced"]->setUniform("camera_zoom", &camera.getZoom());

    // Tiles need to be rendered under the other textures
    this->registry.view<Model, Tile, ToRenderTile>().each([this, &shader_manager](auto& model, auto& tile) {  
        glm::vec4 texture_data = glm::vec4(
            tile.tile_set_texture->frame_data->position.x + tile.position.x, 
            tile.tile_set_texture->frame_data->position.y + tile.position.y, 
            16, 16
        );
        this->renderer.queue(texture_data, model.model, shader_manager["instanced"]);
    });

    this->registry.view<Texture, Model, ToRender>(entt::exclude<Text, Tile>).use<ToRender>().each([this, &shader_manager](const auto entity, auto& texture, auto& model) {  
        glm::vec4 texture_data = glm::vec4(texture.frame_data->position.x, texture.frame_data->position.y, 
            texture.frame_data->size.x, texture.frame_data->size.y
        );
        this->renderer.queue(texture_data, model.model, shader_manager["instanced"]);
    });

    this->renderer.render();
    this->renderer.renderPostProcessing(shader_manager["screen_blur"]);

    shader_manager["instanced_sharp_outline"]->setUniform("P", &camera.getProjectionMatrix()[0][0]);
    shader_manager["instanced_sharp_outline"]->setUniform("V", &camera.getViewMatrix()[0][0]);
    shader_manager["instanced_sharp_outline"]->setUniform("camera_zoom", &camera.getZoom());

    this->registry.view<Texture, Model, ToRender, Outline>(entt::exclude<Text, Tile>).use<ToRender>().each([this, &shader_manager](const auto entity, auto& texture, auto& model) {  
        glm::vec4 texture_data = glm::vec4(texture.frame_data->position.x, texture.frame_data->position.y, 
            texture.frame_data->size.x, texture.frame_data->size.y
        );
        this->renderer.queue(texture_data, model.model, shader_manager["instanced_sharp_outline"]);
    });

    this->renderer.render();

    Camera& gui_camera = registry.ctx().at<Camera&>("gui_camera"_hs);
    shader_manager["instanced"]->setUniform("P", &gui_camera.getProjectionMatrix()[0][0]);
    shader_manager["instanced"]->setUniform("V", &gui_camera.getViewMatrix()[0][0]);
    shader_manager["instanced"]->setUniform("camera_zoom", &camera.getZoom());

    this->registry.view<Texture, Model, GuiElement>().each([this, &shader_manager](const auto entity, auto& texture, auto& model) {  
        glm::vec4 texture_data = glm::vec4(texture.frame_data->position.x, texture.frame_data->position.y, 
            texture.frame_data->size.x, texture.frame_data->size.y
        );
        this->renderer.queue(texture_data, model.model, shader_manager["instanced"]);
    });

    this->renderer.render();

    #ifndef NDEBUG
        shader_manager["instanced_inline"]->setUniform("P", &camera.getProjectionMatrix()[0][0]);
        shader_manager["instanced_inline"]->setUniform("V", &camera.getViewMatrix()[0][0]);
        shader_manager["instanced_inline"]->setUniform("camera_zoom", &camera.getZoom());

        this->registry.view<Collision, Spacial, RenderCollision>().each([this, &shader_manager](
            const auto entity, 
            auto& collision, 
            auto& spacial
        ) {  
            for (auto collision_bounds : collision.bounding_boxes) {
                glm::vec4 texture_data = glm::vec4(
                    0, 0, 
                    collision_bounds.x, collision_bounds.y
                );
                glm::vec3 scale_vector = glm::vec3(spacial.scale.x, spacial.scale.y, spacial.scale.z);
                glm::vec3 dimensions_vector = glm::vec3(collision_bounds.x, collision_bounds.y, 1);
                glm::mat4 scale = glm::scale(glm::mat4(1), scale_vector*dimensions_vector);
                glm::vec3 offset = glm::vec3(collision_bounds.z, collision_bounds.w, 0);
                glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos + (offset*scale_vector));

                this->renderer.queue(texture_data, translate * scale * glm::mat4(1), shader_manager["instanced_inline"]);
            }
        });
        this->renderer.render();
    #endif
    this->renderer.renderPostProcessing(shader_manager["screen"]);
    this->renderer.present(shader_manager["screen"]);
}