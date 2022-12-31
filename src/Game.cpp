#include "Game.h"

Game::Game(SDL_Window* window) : window{ window }{

        using namespace entt::literals;

        this->registry.ctx().emplace<Clock&>(this->clock);
        this->registry.ctx().emplace_hint<Camera&>("worldCamera"_hs, this->worldCamera);
        this->registry.ctx().emplace_hint<Camera&>("guiCamera"_hs, this->guiCamera);
        this->registry.ctx().emplace<InputManager&>(this->inputManager);
        this->registry.ctx().emplace<TextureManager&>(this->textureManager);

        this->systems.push_back(new MapLoaderSystem(this->registry));
        this->systems.push_back(new InputSystem(this->registry));
        this->systems.push_back(new CollisionSystem(this->registry));
        this->systems.push_back(new AnimationSystem(this->registry));
        this->systems.push_back(new MovementSystem(this->registry));
        this->systems.push_back(new CameraSystem(this->registry));
        this->systems.push_back(new GUISystem(this->registry));
        this->systems.push_back(new RenderSystem(this->registry));

        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities
        // this->loadTiledMap("./src/assets/maps/Test/test.tmx");
        
        // // Enable text input
        // SDL_StartTextInput();

        const auto map = this->registry.create();
        this->registry.emplace<MapLoader>(map, "./src/assets/maps/Test/test.tmx");

        entities::TextBox(this->registry, std::string("Hello"), true);
}

void Game::mainLoop() {

    while(!this->inputManager.isQuit()) {

        // printf("\rFPS: %f", this->clock.getSmoothedFPS());

        this->clock.tick();
        this->inputManager.update();

        // std::vector<double> times;

        for (System* system : this->systems) {

            // double start =  SDL_GetPerformanceCounter();
            system->update();
            // double total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;

            // times.push_back(total);

            // std::cout << times.size() - 1 << ": " << total << std::endl;
        }

        // double totalTime = 0.0;

        // for (auto time : times) {
        //     totalTime += time;
        // }

        // std::cout << "Total: " << totalTime << std::endl << std::endl;    

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    
    SDL_StopTextInput();
}