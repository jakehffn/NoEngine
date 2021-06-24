#include "Scene.h"

Scene::Scene(SDL_Window* window) : window{ window }{

        this->renderSystem = new RenderSystem(this->registry);
        this->inputSystem = new InputSystem(this->registry);
        
        // Enable text input
        SDL_StartTextInput();

        create_entity::addPlayer(this->registry, glm::vec3(0, 0, 0));
        create_entity::addBag(this->registry, glm::vec3(300, 300, -0.1));
        create_entity::addVSCodeBackground(this->registry);
}

Scene::~Scene() {
    // Deallocate Program
    glDeleteProgram(programID);
}

void Scene::mainLoop() {

    while(!this->inputSystem->isQuit()) {


        this->clock.tick();

        this->inputSystem->update(this->registry, this->clock.getDeltaTime());
        this->renderSystem->update(this->registry, this->clock);

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    
    SDL_StopTextInput();
}