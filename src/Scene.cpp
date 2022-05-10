#include "Scene.h"

Scene::Scene(SDL_Window* window) : window{ window }{

        this->renderSystem = new RenderSystem(this->registry);
        this->inputSystem = new InputSystem(this->registry);
        this->stateSystem = new StateSystem(this->registry);
        this->collisionSystem = new CollisionSystem(this->registry);

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


        this->clock.tick();

        this->inputSystem->update(this->registry, this->clock.getDeltaTime());
        this->stateSystem->update(this->registry);
        this->collisionSystem->update(this->registry);
        this->renderSystem->update(this->registry, this->clock);
        

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

        // Add all objects from the object layer of the map
        tson::Layer* objectLayer = map->getLayer("Object Layer");
        this->addObjects(objectLayer->getObjects());

        tson::Layer* tileLayer = map->getLayer("Tile Layer");
        this->addTiles(tileLayer->getTileData());

    } else {
        std::cout << map->getStatusMessage();
    }

}

void Scene::addObjects(std::vector<tson::Object> objs) {

    for (auto& obj : objs) {

        tson::ObjectType objType = obj.getObjectType();
        tson::Vector2i pos = obj.getPosition();
        tson::Vector2i size = obj.getSize();

        std::string name = obj.getName();
        entities::create[name](this->registry, glm::vec3(pos.x, pos.y, 0));
    }
}

void Scene::addTiles(std::map<std::tuple<int, int>, tson::Tile*> tileData) {

    std::vector<glm::vec3> tiles;

    for (const auto &[pos, tile] : tileData) {
        
        // Emplace vector containing the position of the tile and the tile id for use in renderer.
        tiles.emplace_back(std::get<0>(pos), std::get<1>(pos), tile->getId() - 1); // ID seems to be off by one for some reason. Not sure why.

        tson::PropertyCollection properties = tile->getProperties();

        if (properties.getSize() == 4) {

            glm::vec2 collisionDim{ properties.getValue<int>("CollisionWidth"), 
                properties.getValue<int>("CollisionHeight") };
            glm::vec2 collisionOffsets{ properties.getValue<int>("CollisionXOffset"), 
                properties.getValue<int>("CollisionYOffset") }; 

            entities::TileEntity(this->registry, pos, tile->getId() - 1, collisionDim, collisionOffsets);

        } else {
            entities::TileEntity(this->registry, pos, tile->getId() - 1);
        }
    }
        this->renderSystem->updateTiles(tiles);
}