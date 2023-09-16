#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "System.hpp"

#include "load_map.hpp"
#include "animation.hpp"

#include "spacial.hpp"
#include "collision.hpp"
#include "collidable.hpp"
#include "tile_set.hpp"
#include "tile.hpp"
#include "renderable.hpp"
#include "text.hpp"
#include "name.hpp"
#include "persistent.hpp"

#include "sprite_sheet_atlas.hpp"
#include "component_grid.hpp"
#include "resource_loader.hpp"

#include "debug_timer.hpp"

class MapLoader {
public:
    MapLoader(entt::registry& registry);
    MapLoader(MapLoader&&) = default;
    MapLoader& operator=(MapLoader&&) = default;

    void loadIfQueued();

    template<auto Func>
    void connectBeforeDestroy() {
        entt::sink sink{this->before_destroy};
        sink.connect<Func>();
    }

    template<auto Func, typename Instance>
    void connectBeforeDestroy(Instance instance) {
        entt::sink sink{this->before_destroy};
        sink.connect<Func>(instance);
    }

    template<auto Func>
    void connectAfterLoad() {
        entt::sink sink{this->after_load};
        sink.connect<Func>();
    }

    template<auto Func, typename Instance>
    void connectAfterLoad(Instance instance) {
        entt::sink sink{this->after_load};
        sink.connect<Func>(instance);
    }
private:
    void queueLoad(entt::registry& registry, entt::entity entity);
    void loadTiledMap(const char* map_path);

    void addObjects(const tmx::Map& map);
    void addObject(const tmx::Map& map, const tmx::Object& object);

    void addTilesets(tmx::Map& map);

    void addCollision(entt::entity entity, const tmx::Tileset::Tile* tile);

    entt::registry& registry;
    entt::entity queued_map_loader_entity{entt::null};

    entt::sigh<void(entt::registry&)> before_destroy;
    entt::sigh<void(entt::registry&)> after_load;
};