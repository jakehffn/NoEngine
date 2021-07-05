#include "Scene.h"

Scene::Scene(SDL_Window* window) : window{ window }{

        this->renderSystem = new RenderSystem(this->registry);
        this->inputSystem = new InputSystem(this->registry);
        this->stateSystem = new StateSystem(this->registry);
        this->collisionSystem = new CollisionSystem(this->registry);

        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities
        this->loadTiledMap("./src/assets/maps/baseMap/baseMap.json");
        
        // Enable text input
        SDL_StartTextInput();

        create_entity::Map1Background(this->registry);
        create_entity::TextBox(this->registry, std::string("1234567890-=[]\\;',./`!@#$%^&*()_+{}|:\"<>?~"));
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

        tson::Layer* objectLayer = map->getLayer("Object Layer");

        for (auto& obj : objectLayer->getObjects()) {
            
            this->addObject(obj);
        }

    } else {
        std::cout << map->getStatusMessage();
    }

}

void Scene::addObject(tson::Object& obj) {

    tson::ObjectType objType = obj.getObjectType();

    tson::Vector2i pos = obj.getPosition();
    tson::Vector2i size = obj.getSize();

    std::unordered_map<std::string, void (*)(entt::registry&, glm::vec3)> nameFuncMap;
    
    nameFuncMap["Player"] = &create_entity::Player;
    nameFuncMap["BoxHead"] = &create_entity::BoxHead;
    nameFuncMap["Bag"] = &create_entity::Bag;

    if (objType == tson::ObjectType::Rectangle) {

        create_entity::CollisionBox(this->registry, glm::vec2(pos.x, pos.y), glm::vec2(size.x, size.y));

    } else {

        std::string name = obj.getName();

        std::cout << name << std::endl;

        nameFuncMap[name](this->registry, glm::vec3(pos.x, pos.y, 0));

    }
}