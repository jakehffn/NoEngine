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

#include "System.h"

#include "map_loader.h"
#include "animation.h"

#include "spacial.h"
#include "collision.h"
#include "tile_set.h"
#include "tile.h"
#include "renderable.h"

#include "texture_atlas.h"
#include "grid.h"

class MapLoaderSystem : public System {
public:
    MapLoaderSystem(entt::registry& registry);

private:
    void update() override;

    void loadTiledMap(const char* mapPath);

    void addObjects(const tmx::Map& map);
    void addObject(const tmx::Map& map, const tmx::Object& object);

    void addTilesets(tmx::Map& map);

    void addCollision(entt::entity entity, const tmx::Tileset::Tile* tile);
};