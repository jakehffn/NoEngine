#include "Scene.h"

Scene::Scene(SDL_Window* window) : window{ window }{

        this->renderSystem = new RenderSystem(this->registry);
        
        // Enable text input
        SDL_StartTextInput();

        const auto entity = this->registry.create();
        registry.emplace<Sprite>(entity, "./src/assets/sprites/BagHead.png");
        registry.emplace<Model>(entity, glm::mat4(10));
        registry.emplace<Spacial>(entity, glm::vec3(100, 100, 0), glm::vec3(0, 0, 0), glm::vec3(320, 320, 0));

        printf("Completed Scene initialization\n");
}

Scene::~Scene() {
    // Deallocate Program
    glDeleteProgram(programID);
}

void Scene::mainLoop() {

    while(!this->input.quitProgram() && !this->input.isKeyDown(SDLK_ESCAPE)) {


        this->clock.tick();
        this->input.update();

        renderSystem->update(this->registry);

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    
    SDL_StopTextInput();
}