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

        // renderGrid.init(mapBounds.width, mapBounds.height, 128);

        this->addObjects(map);
        this->addTilesets(map);
    }
}

void MapLoaderSystem::addObjects(const tmx::Map& map) {

    TextureAtlas& textureAtlas = this->registry.ctx().at<TextureAtlas&>();

    const auto& layers = map.getLayers();

    // Layers -> Objects -> TileSets -> Tile
    //            |-> Properties        |-> Properties
    for (const auto& layer : layers) {

        if (layer->getType() == tmx::Layer::Type::Object) {

            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            
            for (const auto& object : objectLayer.getObjects()) {

                const auto& entity = this->registry.create();

                std::string type = object.getType();
                tmx::Vector2f position = object.getPosition();

                for (const auto& property : object.getProperties()) {
                    ComponentFactory::emplaceComponent(this->registry, entity, property.getName(), std::vector<std::string>());
                }

                int tileID = object.getTileID();

                for (const auto& tileSet : map.getTilesets()) {

                    if (tileSet.hasTile(tileID)) {

                        const auto& tile = tileSet.getTile(tileID);

                        std::string textureName = std::filesystem::path(tile->imagePath).stem().string(); 

                        textureAtlas.initEntity(this->registry, entity, textureName);

                        this->registry.emplace<Spacial>(entity, glm::vec3(position.x, position.y, 0), 
                            glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(tile->imageSize.x, tile->imageSize.y));
                        this->registry.emplace<Renderable>(entity);

                        for (const auto& property : tile->properties) {
                            ComponentFactory::emplaceComponent(this->registry, entity, property.getName(), std::vector<std::string>());
                        }
                    }
                }
            }
        }
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

        // for (const auto& property : tileset.getProperties()) {}

        std::string textureName = std::filesystem::path(tileset.getImagePath()).stem().string(); 

        // std::cout << std::filesystem::canonical(tileSetPath);

        textureAtlas.initEntity(this->registry, tileSetEntity, textureName);
        
        this->registry.emplace<TileSet>(tileSetEntity, (int)dimensions.x, (int)dimensions.y, (int)tileset.getFirstGID(), (int)tileset.getLastGID());

        for (auto tileDataVec : tileDataMap[tileset.getName()]) {

            const tmx::Tileset::Tile* tile = tileset.getTile(tileDataVec.z);

            const auto& tileEntity = this->registry.create();

            glm::vec2 imagePosition = glm::vec2((float)tile->imagePosition.x, (float)tile->imagePosition.y);

            this->registry.emplace<Tile>(tileEntity, (int)tileDataVec.z, imagePosition);
            this->registry.emplace<Spacial>(tileEntity, glm::vec3(tileDataVec.x, tileDataVec.y, 0) * 16.0f, 
                glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(16, 16));
            this->registry.emplace<Renderable>(tileEntity);

            std::vector<glm::vec4> boundingBoxes;

            // Load the collisions boxes associated with the tile, which is located in the tmx::tileset
            for (auto collisionBox : tile->objectGroup.getObjects()) {
                
                tmx::Vector2f position = collisionBox.getPosition();
                tmx::FloatRect rectangle = collisionBox.getAABB();

                boundingBoxes.emplace_back(rectangle.width, rectangle.height, position.x, position.y);
            }

            if (boundingBoxes.size() != 0) {
                this->registry.emplace<Collision>(tileEntity, boundingBoxes);
            }
        }
    }
}