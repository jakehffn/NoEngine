#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <iostream>

#include "System.h"

#include "MapLoader.h"

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
    void addTiles(const tmx::TileLayer& tileLayer, tmx::Vector2u dimensions);
};