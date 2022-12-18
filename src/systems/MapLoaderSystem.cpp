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
                this->addTiles(tileLayer, map.getTileCount());
            }
        }

        const auto& tilesets = map.getTilesets();

        for(const auto& tileset : tilesets) {
            
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

void MapLoaderSystem::addTiles(const tmx::TileLayer& tileLayer, tmx::Vector2u dimensions) {

    int x = 0;
    int y = 0;

    for (const auto& tile : tileLayer.getTiles()) {

        entities::TileEntity(this->registry, {x, y}, tile.ID - 1);

        x++;

        if (x == dimensions.x) {

            x = 0;
            y++;
        }
    }
}