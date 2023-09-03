#include "game.hpp"

Game::Game(SDL_Window* window) : window{ window } {
        using namespace entt::literals;

        this->registry.ctx().emplace<Clock&>(this->clock);
        this->registry.ctx().emplace_hint<Camera&>("world_camera"_hs, this->world_camera);
        this->registry.ctx().emplace_hint<Camera&>("gui_camera"_hs, this->gui_camera);
        this->registry.ctx().emplace<Input&>(this->input_manager);
        this->registry.ctx().emplace<TextureAtlas&>(this->texture_atlas);
        this->registry.ctx().emplace<SpriteSheetAtlas&>(this->sprite_sheet_atlas);
        this->sprite_sheet_atlas.initMissingTextureSpriteSheet(this->registry, "debug/MissingTexture");

        this->registry.ctx().emplace<ComponentGrid<Renderable>&>(this->renderable_grid);
        this->registry.ctx().emplace<ComponentGrid<Collision>&>(this->collision_grid);

        this->systems.push_back(new MapLoaderSystem(this->registry));
        this->systems.push_back(new TextSystem(this->registry));
        this->systems.push_back(new StateMachineSystem(this->registry));
        this->systems.push_back(new InputSystem(this->registry));
        this->systems.push_back(new AnimationSystem(this->registry));
        this->systems.push_back(new MovementSystem(this->registry));
        this->systems.push_back(new CollisionSystem(this->registry));
        this->systems.push_back(new CameraSystem(this->registry));
        this->systems.push_back(new GUISystem(this->registry));

        auto render_system = new RenderSystem(this->registry);
        this->screen_texture = render_system->getRenderer()->getScreenTexture();
        this->systems.push_back(render_system);

        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities

        const auto map = this->registry.create();
        this->registry.emplace<MapLoader>(map, "./assets/maps/Test/test.tmx");
}

inline void Game::startFrame() {
    #ifndef NDEBUG
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    #endif
}

inline void Game::endFrame() {
    #ifndef NDEBUG
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto backup_current_window = SDL_GL_GetCurrentWindow();
            auto backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    #endif
    SDL_GL_SwapWindow(this->window);
}

void Game::mainLoop(void (*debugCallback)()) {
    while(!this->input_manager.isQuit()) {
        this->startFrame();

        this->clock.tick();
        this->input_manager.update();
        this->renderable_grid.update();
        this->collision_grid.update();

        #ifndef NDEBUG
            std::vector<double> times;
        #endif

        for (auto system : this->systems) {
            #ifndef NDEBUG
                double start = SDL_GetPerformanceCounter();
            #endif
            system->update();
            #ifndef NDEBUG
                double total = (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
                times.push_back(total);
            #endif
        }
        #ifndef NDEBUG
            debugCallback();
        #endif
        this->endFrame();
    }
    SDL_StopTextInput();
}