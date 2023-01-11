#include "MapLoaderSystem.h"

#include "ComponentFactory.h"

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

        this->addObjects(map);
        this->addTilesets(map);
    }
}

void MapLoaderSystem::addObjects(const tmx::Map& map) {

    TextureAtlas& textureAtlas = this->registry.ctx().at<TextureAtlas&>();

    const auto& layers = map.getLayers();

    // Layers -> Objects -> TileSets -> Tile
    //            |-> Propterties        |-> Properties
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

        const auto entity = this->registry.create();

        for (const auto& property : tileset.getProperties()) {
            
            if (property.getName() == "RealTileSet") {

                std::string fileName = property.getFileValue();

                size_t dotIndex = fileName.find_last_of("."); 
                std::string textureName = fileName.substr(0, dotIndex); 

                // std::cout << std::filesystem::canonical(tileSetPath);

                textureAtlas.initEntity(this->registry, entity, textureName);
                
                auto& tileSetComponent = this->registry.emplace<TileSet>(entity, (int)dimensions.x, (int)dimensions.y, std::vector<glm::vec3>(tileDataMap[tileset.getName()]), (int)tileset.getFirstGID());

                for (auto tileDataVec : tileDataMap[tileset.getName()]) {

                    const auto tileEntity = this->registry.create();

                    this->registry.emplace<Spacial>(tileEntity, glm::vec3(tileDataVec.x, tileDataVec.y, 0) * 16.0f, 
                        glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec2(16, 16));

                    std::vector<glm::vec4> boundingBoxes;

                    // Load the collisions boxes associated with the tile, which is located in the tmx::tileset
                    for (auto collisionBox : tileset.getTile(tileDataVec.z)->objectGroup.getObjects()) {
                        
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

        
        // std::cout << tileSetComponent.tileData[5].x << " " <<  tileSetComponent.tileData[5].y << " " << tileSetComponent.tileData[5].z;
    }
}