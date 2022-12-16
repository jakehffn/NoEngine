#include "Game.h"

Game::Game(SDL_Window* window) : window{ window }{

        this->registry.ctx().emplace<Clock&>(this->clock);
        this->registry.ctx().emplace<InputManager&>(this->inputManager);
        this->registry.ctx().emplace<TextureManager&>(this->textureManager);

        this->systems.push_back(new InputSystem(this->registry));
        this->systems.push_back(new CollisionSystem(this->registry));
        this->systems.push_back(new AnimationSystem(this->registry));
        this->systems.push_back(new MovementSystem(this->registry));
        this->systems.push_back(new RenderSystem(this->registry));

        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities
        this->loadTiledMap("./src/assets/maps/Test/test.json");
        
        // Enable text input
        SDL_StartTextInput();

        entities::TextBox(this->registry, std::string("Hello"), true);
}

Game::~Game() {
    // Deallocate Program
    glDeleteProgram(programID);
}

void Game::mainLoop() {

    while(!this->inputManager.isQuit()) {

        printf("\rFPS: %f", this->clock.getAverageFPS());

        this->clock.tick();
        this->inputManager.update();

        for (System* system : this->systems) {
            system->update();
        }

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    
    SDL_StopTextInput();
}

void Game::loadTiledMap(const char* mapPath) {

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK) {

        this->addObjects(map->getLayer("Object Layer"));
        this->addTiles(map->getLayer("Tile Layer"));

    } else {
        std::cout << map->getStatusMessage();
    }

}

void Game::addObjects(tson::Layer* objectLayer) {

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

void Game::addTiles(tson::Layer* tileLayer) {

    for (const auto &[pos, tile] : tileLayer->getTileData()) {

        int firstGid = tile->getTileset()->getFirstgid();

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
}