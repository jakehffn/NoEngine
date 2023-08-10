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

#include "map_loader.hpp"
#include "animation.hpp"

#include "spacial.hpp"
#include "collision.hpp"
#include "tile_set.hpp"
#include "tile.hpp"
#include "renderable.hpp"
#include "text.hpp"

#include "sprite_sheet_atlas.hpp"
#include "component_grid.hpp"

class MapLoaderSystem : public System {
public:
    MapLoaderSystem(entt::registry& registry);

private:
    void update() override;

    void loadTiledMap(const char* map_path);

    void addObjects(const tmx::Map& map);
    void addObject(const tmx::Map& map, const tmx::Object& object);

    void addTilesets(tmx::Map& map);

    void addCollision(entt::entity entity, const tmx::Tileset::Tile* tile);
};