#include "MapLoaderSystem.h"

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

        const auto& layers = map.getLayers();

        for (const auto& layer : layers) {

            if (layer->getType() == tmx::Layer::Type::Object) {

                const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
                this->addObjects(objectLayer);

            } else if (layer->getType() == tmx::Layer::Type::Tile) {

                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                this->addTilesets(map, tileLayer);
            }
        }
    }
}

void MapLoaderSystem::addObjects(const tmx::ObjectGroup& objectGroup) {

    for (const auto& object : objectGroup.getObjects()) {

        std::string type = object.getType();
        tmx::Vector2f pos = object.getPosition();
        // tmx::FloatRect rect = object.getAABB();

        std::string name = object.getName();

        if (entities::create.find(name) != entities::create.end()) {
            entities::create[name](this->registry, glm::vec3(pos.x, pos.y, 0));
        } else {
            std::cerr << "entities::create mapping does not exist for name \"" + name + "\"" << std::endl;
        }
    }
}

void MapLoaderSystem::addTilesets(tmx::Map& map, const tmx::TileLayer& tileLayer) {

    // keep map of tilesetname to tiledata vector

    // - Iterate tiles
    // - For each tile, find which tileset it is a part of
    // - add the data to the tiledata vector
    //      at the end, the number of tiles for each tileset is now known
    // - Create tileset entity, 
    // - Iterate over the tiledata, using the tile id to get collisions then 
    //      subtract the firstGID to get the real tile id for the texture

    std::unordered_map<std::string, std::vector<glm::vec3>> tileDataMap;

    int it = 0;

    tmx::Vector2u dimensions = map.getTileCount();
    const auto& tilesets = map.getTilesets();

    // Initial iteration over the tiles in the layer to get the positions and place the data in the map
    for (const auto& tile : tileLayer.getTiles()) {

        for(const auto& tileset : tilesets) {

            if (tileset.hasTile(tile.ID)) {
                tileDataMap[tileset.getName()].emplace_back(it%dimensions.x, (int)(it/dimensions.x), tile.ID);
            }
        }
        it++;
    }

    for(const auto& tileset : tilesets) {

        const auto tilesetEntity = this->registry.create();

        std::filesystem::path tileSetPath;

        for (const auto& property : tileset.getProperties()) {
            
            if (property.getName() == "RealTileSet") {

                tileSetPath = std::filesystem::path(map.getWorkingDirectory() + "/" + property.getFileValue());

                // std::cout << std::filesystem::canonical(tileSetPath);

                this->registry.emplace<Texture>(tilesetEntity, std::filesystem::canonical(tileSetPath).string(), 4);
                this->registry.emplace<Animation>(tilesetEntity);
                auto& tilesetComponent = this->registry.emplace<TileSet>(tilesetEntity, std::vector<glm::vec3>(tileDataMap[tileset.getName()].size()));

                it = 0;

                // Iterate over the tile data in the map for the current tileset, creating the bounding boxes and setting the pointers to the tile ID
                for (auto tileDataVec : tileDataMap[tileset.getName()]) {

                    tilesetComponent.tileData[it] = glm::vec3(tileDataVec.x, tileDataVec.y, tileDataVec.z - tileset.getFirstGID());
                
                    const auto tileEntity = this->registry.create();

                    this->registry.emplace<Tile>(tileEntity, &tilesetComponent.tileData[it].z);
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

                    it++;
                }
            }
        }

        
        // std::cout << tilesetComponent.tileData[5].x << " " <<  tilesetComponent.tileData[5].y << " " << tilesetComponent.tileData[5].z;
    }
}