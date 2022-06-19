#include "Scene.h"

Scene::Scene(SDL_Window* window) : window{ window }{

        this->renderSystem = new RenderSystem(this->registry);
        this->inputSystem = new InputHandler(this->registry);
        this->stateSystem = new StateSystem(this->registry);
        this->collisionSystem = new CollisionSystem(this->registry);

        this->registry.ctx().emplace<Clock&>(this->clock);

        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities
        this->loadTiledMap("./src/assets/maps/Test/test.json");
        
        // Enable text input
        SDL_StartTextInput();

        entities::TextBox(this->registry, std::string("    Hello"), true);
}

Scene::~Scene() {
    // Deallocate Program
    glDeleteProgram(programID);
}

void Scene::mainLoop() {

    while(!this->inputSystem->isQuit()) {

        printf("\rFPS: %f", this->clock.getAverageFPS());

        this->clock.tick();

        this->inputSystem->update(this->registry);
        this->stateSystem->update(this->registry);
        this->collisionSystem->update(this->registry);
        this->renderSystem->update(this->registry);
        

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    
    SDL_StopTextInput();
}

void Scene::loadTiledMap(const char* mapPath) {

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK) {

        this->addObjects(map->getLayer("Object Layer"));
        this->addTiles(map->getLayer("Tile Layer"));

    } else {
        std::cout << map->getStatusMessage();
    }

}

void Scene::addObjects(tson::Layer* objectLayer) {

    for (auto& obj : objectLayer->getObjects()) {

        tson::ObjectType objType = obj.getObjectType();
        tson::Vector2i pos = obj.getPosition();
        tson::Vector2i size = obj.getSize();

        std::string name = obj.getName();

        if (entities::create.find(name) != entities::create.end()) {
            entities::create[name](this->registry, glm::vec3(pos.x, pos.y, 0));
        } else {
            std::cerr << "entities::create mapping does not exist for name \"" + name + "\"" << std::endl;
        }
    }
}

void Scene::addTiles(tson::Layer* tileLayer) {

    std::vector<glm::vec3> tiles;

    for (const auto &[pos, tile] : tileLayer->getTileData()) {

        int firstGid = tile->getTileset()->getFirstgid();

        // Emplace vector containing the position of the tile and the tile id for use in renderer.
        tiles.emplace_back(std::get<0>(pos), std::get<1>(pos), tile->getGid() - firstGid); // ID seems to be off by one for some reason. Not sure why.

        tson::Layer objectGroup = tile->getObjectgroup();
        std::vector<tson::Object> tileCollisions = objectGroup.getObjects();

        if (!tileCollisions.size() == 0) {

            std::vector<glm::vec4> collisionBoxes;

            for (auto tileCollision : tileCollisions) {

                tson::Vector2i dim = tileCollision.getSize();
                tson::Vector2i offset = tileCollision.getPosition();

                collisionBoxes.emplace_back(dim.x, dim.y, offset.x, offset.y);
            }

            entities::TileEntity(this->registry, pos, tile->getGid() - firstGid, collisionBoxes);

        } else {
            entities::TileEntity(this->registry, pos, tile->getGid() - firstGid);
        }
    }
        this->renderSystem->updateTiles(tiles);
}