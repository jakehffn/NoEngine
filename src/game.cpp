#include "game.hpp"

Game::Game(SDL_Window* window) : window{ window } {
        using namespace entt::literals;

        this->renderable_grid.init(3200, 3200, 16);
        this->collision_grid.init(3200, 3200, 16);

        this->registry.ctx().emplace<Clock&>(this->clock);
        this->registry.ctx().emplace_hint<Camera&>("world_camera"_hs, this->world_camera);
        this->registry.ctx().emplace_hint<Camera&>("gui_camera"_hs, this->gui_camera);
        this->registry.ctx().emplace<Input&>(this->input_manager);
        this->registry.ctx().emplace<TextureAtlas&>(this->texture_atlas);
        this->registry.ctx().emplace<SpriteSheetAtlas&>(this->sprite_sheet_atlas);
        this->sprite_sheet_atlas.initMissingTextureSpriteSheet(this->registry, "debug/MissingTexture");
        this->registry.ctx().emplace<ComponentGrid<Renderable>&>(this->renderable_grid);
        this->registry.ctx().emplace<ComponentGrid<Collision>&>(this->collision_grid);
        this->registry.ctx().emplace<ShaderManager&>(this->shader_manager);
        this->registry.ctx().emplace<MapLoader&>(this->map_loader);
        this->registry.ctx().emplace<ResourceLoader&>(this->resource_loader);
        this->registry.ctx().emplace<TextManager&>(this->text_manager);

        this->systems.push_back(new StateMachineSystem(this->registry));
        this->systems.push_back(new InputSystem(this->registry));
        this->systems.push_back(new AnimationSystem(this->registry));
        this->systems.push_back(new MovementSystem(this->registry));
        this->systems.push_back(new CollisionSystem(this->registry));
        this->systems.push_back(new CameraSystem(this->registry));
        this->systems.push_back(new GuiSystem(this->registry));

        auto render_system = new RenderSystem(this->registry);
        this->screen_texture = render_system->getRenderer()->getScreenTexture();
        this->systems.push_back(render_system);

        this->text_manager.loadFont("./assets/fonts/cozette/cozette.bdf", "Cozette");
        // Tiled map must be loaded after systems are created in order for observers to be able to
        //  monitor patches during creation of entities

        const auto map = this->registry.create();
        this->map_loader.queueLoad("./assets/maps/Test/test.tmx");

        // auto entity = this->registry.create();
        // ResourceLoader::create(registry, entity, "FpsCounterEntity");
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
        {
            DEBUG_TIMER(_, "Main Loop");
            {
                DEBUG_TIMER(context_timer, "Context Updates");
                this->clock.tick();
                this->map_loader.loadIfQueued();
                this->input_manager.update();
                this->renderable_grid.update();
                this->collision_grid.update();
                this->text_manager.update();
            }
            {
                DEBUG_TIMER(systems_timer, "Systems Updates");
                for (auto system : this->systems) {
                    system->update();
                }
            }   
            auto to_destroy = this->registry.view<Destroy>();
            this->registry.destroy(to_destroy.begin(), to_destroy.end());
            #ifndef NDEBUG
                debugCallback();
            #endif
        }
        this->endFrame();
    }
    SDL_StopTextInput();
}