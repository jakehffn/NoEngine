#include "map_loader_system.hpp"

#include "component_factory.hpp"

MapLoaderSystem::MapLoaderSystem(entt::registry& registry) : System(registry) {}

void MapLoaderSystem::update() {

    auto map_loaders = this->registry.view<MapLoader>();

    for (auto entity : map_loaders) {

        auto& map_loader = map_loaders.get<MapLoader>(entity);
        
        this->loadTiledMap(map_loader.map_path.c_str());

        this->registry.destroy(entity);
    }

}

void MapLoaderSystem::loadTiledMap(const char* map_path) {

    tmx::Map map;

    if (map.load(map_path)) {

        tmx::FloatRect map_bounds = map.getBounds();

        auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable, Collision>&>();

        // printf("Map Hidth:%f, Map Height:%f\n", map_bounds.width, mapbounds.height);

        component_grid.init(map_bounds.width, map_bounds.height, 16);

        this->addObjects(map);
        this->addTilesets(map);
    }
}

void MapLoaderSystem::addObjects(const tmx::Map& map) {

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

void MapLoaderSystem::addObject(const tmx::Map& map, const tmx::Object& object) {
        
    const auto& entity = this->registry.create();
    TextureAtlas& texture_atlas = this->registry.ctx().at<TextureAtlas&>();

    std::string type = object.getType();
    tmx::Vector2f position = object.getPosition();

    // These are the properties that were added to the specific tile instance
    for (const auto& property : object.getProperties()) {
        ComponentFactory::emplaceComponent(this->registry, entity, property.getName(), std::vector<std::string>());
    }

    int tile_id = object.getTileID();

    for (const auto& tile_set : map.getTilesets()) {

        if (!tile_set.hasTile(tile_id)) {
            continue;
        }

        const auto& tile = tile_set.getTile(tile_id);

        std::string texture_name = std::filesystem::path(tile->imagePath).stem().string(); 

        texture_atlas.initEntity(this->registry, entity, texture_name);

        this->registry.emplace<Spacial>(entity, glm::vec3(position.x, position.y, 1), 
            glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(tile->imageSize.x, tile->imageSize.y));
        this->registry.emplace<Renderable>(entity);

        // These are the properties that were added the all instances of that tile
        for (const auto& property : tile->properties) {
            ComponentFactory::emplaceComponent(this->registry, entity, property.getName(), std::vector<std::string>());
        }

        this->addCollision(entity, tile);

        // No need to search anymore if the tile has been found in a tile_set
        break;
    }
}

void MapLoaderSystem::addTilesets(tmx::Map& map) {

    TextureAtlas& texture_atlas = this->registry.ctx().at<TextureAtlas&>();

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

        std::string texture_name = std::filesystem::path(tile_set.getImagePath()).stem().string(); 

        // Animations and textures for tiles are handled by the tile_set. 
        //      Animations for all tiles in the tile_set can then be done at once
        //      The drawback for this is that the textures coordinates are calculated every frame
        //      Another drawback is that adding the buffer data for tiles and other entities ends 
        //      up being different.
        texture_atlas.initTileSet(this->registry, tile_set_entity, texture_name);
        
        this->registry.emplace<TileSet>(tile_set_entity, (int)dimensions.x, (int)dimensions.y, (int)tile_set.getFirstGID(), (int)tile_set.getLastGID());

        for (auto tile_data_vec : tile_data_map[tile_set.getName()]) {

            const tmx::Tileset::Tile* tile = tile_set.getTile(tile_data_vec.z);

            const auto& tile_entity = this->registry.create();

            // The position of the tile texture in the tile_set image.
            glm::vec2 image_position = glm::vec2((float)tile->imagePosition.x, (float)tile->imagePosition.y);

            texture_atlas.initTile(this->registry, tile_entity, tile_set_entity, image_position, (int)tile_data_vec.z);

            this->registry.emplace<Tile>(tile_entity, (int)tile_data_vec.z);
            this->registry.emplace<Spacial>(tile_entity, glm::vec3(tile_data_vec.x, tile_data_vec.y, 0) * 16.0f, 
                glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(16, 16));
            this->registry.emplace<Renderable>(tile_entity);

            this->addCollision(tile_entity, tile);
        }
    }
}

void MapLoaderSystem::addCollision(entt::entity entity, const tmx::Tileset::Tile* tile) {

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