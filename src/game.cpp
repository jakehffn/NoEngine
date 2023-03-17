#include "game.hpp"

Game::Game(SDL_Window* window) : window{ window } {

        using namespace entt::literals;

        this->registry.ctx().emplace<Clock&>(this->clock);
        this->registry.ctx().emplace_hint<Camera&>("world_camera"_hs, this->world_camera);
        this->registry.ctx().emplace_hint<Camera&>("gui_camera"_hs, this->gui_camera);
        this->registry.ctx().emplace<Input&>(this->input_manager);
        this->registry.ctx().emplace<TextureAtlas&>(this->texture_atlas);
        this->registry.ctx().emplace<SpriteSheetAtlas&>(this->sprite_sheet_atlas);
        this->registry.ctx().emplace<ComponentGrid<Renderable, Collision>&>(this->component_grid);

        this->systems.push_back(new MapLoaderSystem(this->registry));
        this->systems.push_back(new TextSystem(this->registry));
        this->systems.push_back(new InputSystem(this->registry));
        this->systems.push_back(new AnimationSystem(this->registry));
        this->systems.push_back(new MovementSystem(this->registry));
        this->systems.push_back(new CollisionSystem(this->registry));
        this->systems.push_back(new CameraSystem(this->registry));
        this->systems.push_back(new GUISystem(this->registry));
        this->systems.push_back(new RenderSystem(this->registry));

        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities
    
        // // Enable text input
        // SDL_StartTextInput();

        const auto map = this->registry.create();
        this->registry.emplace<MapLoader>(map, "./assets/maps/Test/test.tmx");

        // const auto fps_counter = this->registry.create();
        // this->registry.emplace<Text>(fps_counter, "", true);
        // this->registry.emplace<FPSCounter>(fps_counter);
        // this->registry.emplace<Spacial>(fps_counter, glm::vec3(10), glm::vec3(0), glm::vec3(1), glm::vec2(100, 100));
}

void Game::mainLoop() {

    // int it = 0;

    while(!this->input_manager.isQuit()) {

        // it++;

        // if (it > 200) {

        //     printf("\rFPS: %f", this->clock.getSmoothedFPS());
        //     it=0;
        // }

        this->clock.tick();
        this->input_manager.update();
        this->component_grid.update();

        std::vector<double> times;

        for (auto system : this->systems) {

            double start =  SDL_GetPerformanceCounter();
            system->update();
            double total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;

            times.push_back(total);

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

    #ifndef NDEBUG
        std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    #endif
    
    SDL_StopTextInput();
}