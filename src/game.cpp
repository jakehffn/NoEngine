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

        const auto map = this->registry.create();
        this->registry.emplace<MapLoader>(map, "./assets/maps/Test/test.tmx");
}

void Game::mainLoop() {

    while(!this->input_manager.isQuit()) {

        this->clock.tick();
        this->input_manager.update();
        this->component_grid.update();

        #ifndef NDEBUG
            std::vector<double> times;
        #endif

        for (auto system : this->systems) {

            #ifndef NDEBUG
                double start =  SDL_GetPerformanceCounter();
            #endif
            system->update();
            #ifndef NDEBUG
                double total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
                times.push_back(total);
            #endif
        }
        // Update screen
        SDL_GL_SwapWindow(window);
    }

    #ifndef NDEBUG
        std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";
    #endif
    
    SDL_StopTextInput();
}