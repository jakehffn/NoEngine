#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <filesystem>

#include "System.h"

#include "MapLoader.h"
#include "TileSet.h"
#include "Animation.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "entities.h"

class MapLoaderSystem : public System {
public:
    MapLoaderSystem(entt::registry& registry);

private:
    void update() override;

    void loadTiledMap(const char* mapPath);

    void addObjects(const tmx::ObjectGroup& objectGroup);
    void addTilesets(tmx::Map& map, const tmx::TileLayer& tileLayer);
};