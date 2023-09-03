#include "render_system.hpp"

RenderSystem::RenderSystem(entt::registry& registry) : System(registry), 
    spacial_observer{entt::observer(registry, entt::collector.update<Spacial>().where<Texture>())},
    texture_observer{entt::observer(registry, entt::collector.update<Texture>().where<Spacial>())} {
}

void RenderSystem::update() {
    DEBUG_TIMER(_, "RenderSystem::update");

    this->cullEntities();
    this->sortEntities();
    this->updateModels();
    this->bufferEntityData();
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

    glm::vec2 camera_dimensions = camera.getCameraDim();
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
        
        return lhSpacial.pos.y + lhSpacial.dim.y < rhSpacial.pos.y + rhSpacial.dim.y;

    }, entt::insertion_sort {}); // Insertion sort is much faster as the spacials will generally be "mostly sorted"
}

void RenderSystem::updateModels() {
    DEBUG_TIMER(_, "RenderSystem::updateModels");
    // TDOD: Consider only updating models of entities which need rendering

    // Update the models of all the entities whose spacials have been changed
    this->spacial_observer.each([this](entt::entity entity){
        auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
        this->registry.emplace_or_replace<Model>(entity, this->getModel(spacial, texture));
    });
    // ...and same for models with updated textures
    // TODO: Consider ways of avoiding overlap between these two groups
    this->texture_observer.each([this](entt::entity entity){
        auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
        this->registry.emplace_or_replace<Model>(entity, this->getModel(spacial, texture));
    });

    // Create models on entities which have never been rendered
    auto no_model_entities = this->registry.view<Spacial, Texture>(entt::exclude<Model>);
    for (const auto entity : no_model_entities) {
        auto [spacial, texture] = no_model_entities.get<Spacial, Texture>(entity);
        this->registry.emplace<Model>(entity, this->getModel(spacial, texture));
    }

    auto no_model_tiles = this->registry.view<Spacial, Tile>(entt::exclude<Model>);
    for (const auto entity : no_model_tiles) {
        auto [spacial, tile] = no_model_tiles.get<Spacial, Tile>(entity);
        this->registry.emplace<Model>(entity, this->getTileModel(spacial));
    }
}

glm::mat4 RenderSystem::getModel(const Spacial& spacial, const Texture& texture) {
    // The model does not represent the physical location exactly, but the rendered location
    //  Information from the texture is needed so that the sprite can be placed correctly
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 rotate = glm::mat4(1.0f);
    
    rotate = glm::rotate(rotate, spacial.rot.x, glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, spacial.rot.y, glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, spacial.rot.z, glm::vec3(0, 0, 1));


    glm::vec3 scale_vector = glm::vec3(spacial.scale.x, spacial.scale.y, spacial.scale.z);
    glm::vec3 dimensions_vector = glm::vec3(texture.frame_data->size.x, texture.frame_data->size.y, 1);

    glm::mat4 scale = glm::scale(glm::mat4(1), scale_vector*dimensions_vector);

    glm::vec3 offset = glm::vec3(texture.frame_data->offset.x, texture.frame_data->offset.y, 0);

    glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos + (offset*scale_vector));

    // Order matters
    return (translate * scale * rotate);
}

glm::mat4 RenderSystem::getTileModel(const Spacial& spacial) {
    AtlasData tile_frame_data = {
        {0, 0},
        {16, 16},
        {0, 0}
    };
    return this->getModel(spacial, {"", &tile_frame_data});
}

void RenderSystem::bufferEntityData() {
    // Tiles need to be rendered under the other textures
    this->registry.view<Model, Tile, ToRenderTile>().each([this](auto& model, auto& tile) {  
        glm::vec4 texture_data = glm::vec4(
            tile.tile_set_texture->frame_data->position.x + tile.position.x, 
            tile.tile_set_texture->frame_data->position.y + tile.position.y, 
            16, 16
        );
        this->renderer.addBufferData(texture_data, model.model);
    });

    this->registry.view<Texture, Model, ToRender>(entt::exclude<Text, Tile>).use<ToRender>().each([this](auto& texture, auto& model) {  
        glm::vec4 texture_data = glm::vec4(texture.frame_data->position.x, texture.frame_data->position.y, 
            texture.frame_data->size.x, texture.frame_data->size.y);

        this->renderer.addBufferData(texture_data, model.model);
    });
}

void RenderSystem::render() {
    using namespace entt::literals;
    
    Camera camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    TextureAtlas& texture_atlas = this->registry.ctx().at<TextureAtlas&>();
    Clock clock = this->registry.ctx().at<Clock&>();

    this->renderer.render(camera.getViewMatrix(), camera.getProjectionMatrix(),
        glm::vec2(texture_atlas.width, texture_atlas.height), 
        texture_atlas.gl_texture_id, clock.getCumulativeTime());
}