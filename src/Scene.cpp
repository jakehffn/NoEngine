#include "Scene.h"

Scene::Scene(SDL_Window* window) : window{ window }{

        this->renderSystem = new RenderSystem(this->registry);
        this->inputSystem = new InputSystem(this->registry);
        
        // Enable text input
        SDL_StartTextInput();

        const auto player = this->registry.create();
        registry.emplace<Sprite>(player, "./src/assets/sprites/BagHead.png");
        registry.emplace<Model>(player, glm::mat4(10));
        registry.emplace<Spacial>(player, glm::vec3(100, 100, 0), glm::vec3(0, 0, 0), glm::vec3(5, 5, 0));
        registry.emplace<Input>(player, 1000.0f);
        registry.emplace<CameraController>(player, 100.0f);

        const auto bag = this->registry.create();
        registry.emplace<Sprite>(bag, "./src/assets/sprites/Bag.png");
        registry.emplace<Model>(bag, glm::mat4(10));
        registry.emplace<Spacial>(bag, glm::vec3(0, 0, -.2), glm::vec3(0, 0, 0), glm::vec3(1, 5, 0));
        registry.emplace<Animation>(bag, 5, std::vector<int>{0,0,0,0,0,0,0,0,1,0,0,1,2,3,4});

        const auto background = this->registry.create();
        registry.emplace<Sprite>(background, "./src/assets/sprites/ScreenShot (76).png");
        registry.emplace<Model>(background, glm::mat4(10));
        registry.emplace<Spacial>(background, glm::vec3(0, 0, -.5), glm::vec3(0, 0, 0), glm::vec3(5, 5, 0));

        printf("Completed Scene initialization\n");
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