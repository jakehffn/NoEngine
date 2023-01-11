#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "System.h"

#include "MapLoader.h"
#include "Animation.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "Spacial.h"
#include "Collision.h"
#include "TextureAtlas.h"
#include "TileSet.h"

class MapLoaderSystem : public System {
public:
    MapLoaderSystem(entt::registry& registry);

private:
    void update() override;

    void loadTiledMap(const char* mapPath);

    void addObjects(const tmx::Map& map);
    void addTilesets(tmx::Map& map);
};