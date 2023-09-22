#include "map_loader.hpp"

MapLoader::MapLoader(entt::registry& registry) : registry{registry} {}

void MapLoader::queueLoad(const char* map_path) {
    this->queued_map_path = map_path;
}

void MapLoader::loadIfQueued() {
    if (this->queued_map_path != NULL) {
        this->loadTiledMap(this->queued_map_path);
    }
    this->queued_map_path = NULL;
}

void MapLoader::loadTiledMap(const char* map_path) {
    tmx::Map map;

    if (map.load(map_path)) {
        tmx::FloatRect map_bounds = map.getBounds();

        this->before_destroy.publish(this->registry);

        for (auto entity : this->registry.view<Collision>(entt::exclude<ComponentGridIgnore, Persistent>)) {
            this->registry.remove<Collision>(entity);
        }

        for (auto entity : this->registry.view<Renderable>(entt::exclude<ComponentGridIgnore, Persistent>)) {
            this->registry.remove<Renderable>(entity);
        }

        for (auto entity : this->registry.view<entt::entity>()) {
            if (this->registry.valid(entity) && !this->registry.all_of<Persistent>(entity)) {
                this->registry.destroy(entity);
            }
        }

        this->addObjects(map);
        this->addTilesets(map);

        this->after_load.publish(this->registry);

        auto& texture_atlas = this->registry.ctx().at<TextureAtlas&>();
        // TODO: Make a better name for that function. It's not really descriptive of what that does
        texture_atlas.updateAtlas();
    }
}

void MapLoader::addObjects(const tmx::Map& map) {
    const auto& layers = map.getLayers();

    for (const auto& layer : layers) {
        // Skip if not an object layer
        if (layer->getType() != tmx::Layer::Type::Object) {
            continue;
        }  

        const auto& object_layer = layer->getLayerAs<tmx::ObjectGroup>();

        for (const auto& object : object_layer.getObjects()) {
            this->addObject(map, object);
        }  
    }
}

void MapLoader::addObject(const tmx::Map& map, const tmx::Object& object) {
    const auto& entity = this->registry.create();
    
    tmx::Vector2f position = object.getPosition();

    int tile_id = object.getTileID();

    if (tile_id == 0) {
        switch(object.getShape()) {
            case tmx::Object::Shape::Text:
                this->registry.emplace<Spacial>(entity, glm::vec3(position.x, position.y, 1));
                this->registry.emplace<Renderable>(entity);
                this->registry.emplace<Text>(entity, object.getText().content);
                break;
        };
    } else {
        for (const auto& tile_set : map.getTilesets()) {
            if (!tile_set.hasTile(tile_id)) {
                continue;
            }
            const auto& tile = tile_set.getTile(tile_id);

            std::string prefab_name;

            for (auto property : tile->properties) {
                if (property.getName() == "Prefab") {
                    prefab_name = property.getStringValue();
                }
            }

            // Prefab property on specific object will take precedence over the tile
            for (auto property : object.getProperties()) {
                if (property.getName() == "Prefab") {
                    prefab_name = property.getStringValue();
                }
            }

            this->registry.emplace<Spacial>(
                entity, 
                glm::vec3(position.x, position.y, 1), 
                glm::vec2(tile->imageSize.x, tile->imageSize.y)
            );
            this->addCollision(entity, tile);
            
            // Prefab goes last so things can be removed if needed
            registry.emplace<LoadPrefab>(entity, prefab_name, tile->imagePath);
            // No need to search anymore if the tile has been found in a tile_set
            break;
        }
    }
}

void MapLoader::addTilesets(tmx::Map& map) {
    auto& sprite_sheet_atlas = this->registry.ctx().at<SpriteSheetAtlas&>();

    std::unordered_map<std::string, std::vector<glm::vec3>> tile_data_map;

    int it = 0;

    tmx::Vector2u dimensions = map.getTileCount();
    const auto& tile_sets = map.getTilesets();

    const auto& layers = map.getLayers();

    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tile_layer = layer->getLayerAs<tmx::TileLayer>();
            // Initial iteration over the tiles in the layer to get the positions and place the data in the map
            for (const auto& tile : tile_layer.getTiles()) {
                for(const auto& tile_set : tile_sets) {
                    if (tile_set.hasTile(tile.ID)) {
                        tile_data_map[tile_set.getName()].emplace_back(it%dimensions.x, (int)(it/dimensions.x), tile.ID);
                    }
                }
                it++;
            }
        }
    }
    for(const auto& tile_set : tile_sets) {
        // If the tile_set is a collection of images, there will be no ImagePath
        if (tile_set.getImagePath() == "") {
            continue;
        }
        const auto& tile_set_entity = this->registry.create();

        std::string sprite_sheet_name = std::filesystem::path(tile_set.getImagePath()).stem().string(); 
        auto resource_folder_nonrelative = globals::RESOURCE_FOLDER.substr(2);

        std::string specific_resource_path{tile_set.getImagePath()};
        size_t resource_id_start = specific_resource_path.find(resource_folder_nonrelative) + 
            resource_folder_nonrelative.length();

        std::string resource_id = specific_resource_path.substr(
            resource_id_start,
            specific_resource_path.find_last_of('/') - resource_id_start
        );
        
        this->registry.emplace<TileSet>(tile_set_entity, (int)dimensions.x, (int)dimensions.y, (int)tile_set.getFirstGID(), (int)tile_set.getLastGID());
        // Animations and textures for tiles are handled by the tile_set. 
        //      Animations for all tiles in the tile_set can then be done at once
        //      The drawback for this is that the textures coordinates are calculated every frame
        //      Another drawback is that adding the buffer data for tiles and other entities ends 
        //      up being different.
        auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, resource_id);
        auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());
        auto& tile_set_texure = this->registry.emplace<Texture>(tile_set_entity, sprite_sheet_name, default_animation.frames[0]);
        auto& animator = this->registry.emplace<Animator>(tile_set_entity, &default_animation.frame_durations);
        this->registry.emplace<Animation>(tile_set_entity, &animator, &default_animation);
        
        for (auto tile_data_vec : tile_data_map[tile_set.getName()]) {
            const tmx::Tileset::Tile* tile = tile_set.getTile(tile_data_vec.z);

            const auto& tile_entity = this->registry.create();

            // The position of the tile texture in the tile_set image.
            glm::vec2 image_position = glm::vec2((float)tile->imagePosition.x, (float)tile->imagePosition.y);

            this->registry.emplace<Spacial>(
                tile_entity, 
                glm::vec3(tile_data_vec.x, tile_data_vec.y, 0) * 16.0f, 
                glm::vec2(16, 16)
            );
            this->registry.emplace<Tile>(tile_entity, (int)tile_data_vec.z, image_position, &tile_set_texure);
            this->registry.emplace<Renderable>(tile_entity);

            this->addCollision(tile_entity, tile);
        }
    }
}

void MapLoader::addCollision(entt::entity entity, const tmx::Tileset::Tile* tile) {
    std::vector<glm::vec4> bounding_boxes;

    // Load the collisions boxes associated with the tile, which is located in the tmx::tile_set
    for (auto collision_box : tile->objectGroup.getObjects()) {
        tmx::Vector2f position = collision_box.getPosition();
        tmx::FloatRect rectangle = collision_box.getAABB();

        bounding_boxes.emplace_back(rectangle.width, rectangle.height, position.x, position.y);
    }
    if (bounding_boxes.size() != 0) {
        this->registry.emplace<Collision>(entity, std::move(bounding_boxes));
    }
}