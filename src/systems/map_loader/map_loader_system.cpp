#include "map_loader_system.h"

#include "component_factory.h"

MapLoaderSystem::MapLoaderSystem(entt::registry& registry) : System(registry) {}

void MapLoaderSystem::update() {

    auto mapLoaders = this->registry.view<MapLoader>();

    for (auto entity : mapLoaders) {

        auto& mapLoader = mapLoaders.get<MapLoader>(entity);
        
        this->loadTiledMap(mapLoader.mapPath.c_str());

        this->registry.destroy(entity);
    }

}

void MapLoaderSystem::loadTiledMap(const char* mapPath) {

    tmx::Map map;

    if (map.load(mapPath)) {

        tmx::FloatRect mapBounds = map.getBounds();

        auto& renderGrid = this->registry.ctx().at<ComponentGrid<Renderable, Collision>&>();

        // printf("Map Hidth:%f, Map Height:%f\n", mapBounds.width, mapbounds.height);

        renderGrid.init(mapBounds.width, mapBounds.height, 16);

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

        const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();

        for (const auto& object : objectLayer.getObjects()) {

            this->addObject(map, object);
        }  
    }
}

void MapLoaderSystem::addObject(const tmx::Map& map, const tmx::Object& object) {
        
    const auto& entity = this->registry.create();
    TextureAtlas& textureAtlas = this->registry.ctx().at<TextureAtlas&>();

    std::string type = object.getType();
    tmx::Vector2f position = object.getPosition();

    // These are the properties that were added to the specific tile instance
    for (const auto& property : object.getProperties()) {
        ComponentFactory::emplaceComponent(this->registry, entity, property.getName(), std::vector<std::string>());
    }

    int tileID = object.getTileID();

    for (const auto& tileSet : map.getTilesets()) {

        if (!tileSet.hasTile(tileID)) {
            continue;
        }

        const auto& tile = tileSet.getTile(tileID);

        std::string textureName = std::filesystem::path(tile->imagePath).stem().string(); 

        textureAtlas.initEntity(this->registry, entity, textureName);

        this->registry.emplace<Spacial>(entity, glm::vec3(position.x, position.y, 1), 
            glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(tile->imageSize.x, tile->imageSize.y));
        this->registry.emplace<Renderable>(entity);

        // These are the properties that were added the all instances of that tile
        for (const auto& property : tile->properties) {
            ComponentFactory::emplaceComponent(this->registry, entity, property.getName(), std::vector<std::string>());
        }

        this->addCollision(entity, tile);

        // No need to search anymore if the tile has been found in a tileset
        break;
    }
}

void MapLoaderSystem::addTilesets(tmx::Map& map) {

    TextureAtlas& textureAtlas = this->registry.ctx().at<TextureAtlas&>();

    std::unordered_map<std::string, std::vector<glm::vec3>> tileDataMap;

    int it = 0;

    tmx::Vector2u dimensions = map.getTileCount();
    const auto& tilesets = map.getTilesets();

    const auto& layers = map.getLayers();

    for (const auto& layer : layers) {

        if (layer->getType() == tmx::Layer::Type::Tile) {

            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            // Initial iteration over the tiles in the layer to get the positions and place the data in the map
            for (const auto& tile : tileLayer.getTiles()) {

                for(const auto& tileset : tilesets) {

                    if (tileset.hasTile(tile.ID)) {
                        tileDataMap[tileset.getName()].emplace_back(it%dimensions.x, (int)(it/dimensions.x), tile.ID);
                    }
                }
                it++;
            }
        }
    }

    for(const auto& tileset : tilesets) {

        // If the tileset is a collection of images, there will be no ImagePath
        if (tileset.getImagePath() == "") {
            continue;
        }

        const auto& tileSetEntity = this->registry.create();

        std::string textureName = std::filesystem::path(tileset.getImagePath()).stem().string(); 

        // Animations and textures for tiles are handled by the tileset. 
        //      Animations for all tiles in the tileset can then be done at once
        //      The drawback for this is that the textures coordinates are calculated every frame
        //      Another drawback is that adding the buffer data for tiles and other entities ends 
        //      up being different.
        textureAtlas.initTileSet(this->registry, tileSetEntity, textureName);
        
        this->registry.emplace<TileSet>(tileSetEntity, (int)dimensions.x, (int)dimensions.y, (int)tileset.getFirstGID(), (int)tileset.getLastGID());

        for (auto tileDataVec : tileDataMap[tileset.getName()]) {

            const tmx::Tileset::Tile* tile = tileset.getTile(tileDataVec.z);

            const auto& tileEntity = this->registry.create();

            // The position of the tile texture in the tileset image.
            glm::vec2 imagePosition = glm::vec2((float)tile->imagePosition.x, (float)tile->imagePosition.y);

            textureAtlas.initTile(this->registry, tileEntity, tileSetEntity, imagePosition, (int)tileDataVec.z);

            this->registry.emplace<Tile>(tileEntity, (int)tileDataVec.z);
            this->registry.emplace<Spacial>(tileEntity, glm::vec3(tileDataVec.x, tileDataVec.y, 0) * 16.0f, 
                glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(16, 16));
            this->registry.emplace<Renderable>(tileEntity);

            this->addCollision(tileEntity, tile);
        }
    }
}

void MapLoaderSystem::addCollision(entt::entity entity, const tmx::Tileset::Tile* tile) {

    std::vector<glm::vec4> bounding_boxes;

    // Load the collisions boxes associated with the tile, which is located in the tmx::tileset
    for (auto collisionBox : tile->objectGroup.getObjects()) {
        
        tmx::Vector2f position = collisionBox.getPosition();
        tmx::FloatRect rectangle = collisionBox.getAABB();

        bounding_boxes.emplace_back(rectangle.width, rectangle.height, position.x, position.y);
    }

    if (bounding_boxes.size() != 0) {
        this->registry.emplace<Collision>(entity, std::move(bounding_boxes));
    }
}