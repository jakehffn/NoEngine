#include "debug.hpp"
#include "debug_timer.hpp"

DebugWindow::DebugWindow(Game* game) : game{game} {}

void DebugWindow::show() {
    if (ImGui::Begin("Debug", NULL, 
        ImGuiWindowFlags_MenuBar
    )) {
        this->showMenuBar();
        if (this->open_texture_atlas) {
            this->showTextureAtlas();
        }
        if (this->open_entity_viewer) {
            this->showEntityViewer();
        }
        if (this->open_shader_viewer) {
            this->showShaderViewer();
        }
        this->showMainWindow();
    }
    ImGui::End();
}

void DebugWindow::showMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{8, 8});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{6, 6});
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("View", true)) {
            ImGui::MenuItem("Texture Atlas", "", &this->open_texture_atlas, true);
            ImGui::MenuItem("Entity Viewer", "", &this->open_entity_viewer, true);
            ImGui::MenuItem("Timer Window", "", &DebugTimer::open_timers_window, true);
            ImGui::MenuItem("Shader Viewer", "", &this->open_shader_viewer, true);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar(2);
}

void DebugWindow::showMainWindow() {
    if (!this->show_collision_boxes) {
        if (ImGui::Button("Show Collision")) {
            this->show_collision_boxes = true;
            this->game->registry.view<Collision>().each([this](const entt::entity entity, auto collision) {
                this->game->registry.emplace_or_replace<RenderCollision>(entity);
            });
            this->game->registry.on_construct<Collision>().
                connect<&entt::registry::emplace_or_replace<RenderCollision>>();
        }
    } else {
        if (ImGui::Button("Hide Collision")) {
            this->show_collision_boxes = false;
            this->game->registry.view<Collision>().each([this](const entt::entity entity, auto collision) {
                this->game->registry.remove<RenderCollision>(entity);
            });
            this->game->registry.on_construct<Collision>().
                disconnect<&entt::registry::emplace_or_replace<RenderCollision>>();
        }
    }
    ImGui::SameLine();
    ImGui::Text("%.2f", ImGui::GetIO().Framerate);
}

void DebugWindow::showTextureAtlas() {
    ImGui::SetNextWindowSize({0, 0}, 0);
    if (ImGui::Begin("Texture Atlas", &this->open_texture_atlas, 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    )) {
        const float scale = 2.0;
        ImGui::Image(
            (void*)(intptr_t)this->game->texture_atlas.gl_texture_id,
            {(float)this->game->texture_atlas.width * scale, (float)this->game->texture_atlas.height * scale},
            {0, 0},
            {1, 1}
        );
    }
    ImGui::End();
}

void DebugWindow::showEntityViewer() {
    if (!this->game->registry.valid(this->selected_entity)) {
        this->selected_entity = entt::null;
    }

    ImGui::SetNextWindowSize({800, 440}, 0);
    if (ImGui::Begin("Entity Viewer", &this->open_entity_viewer, 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    )) {
        ImGui::BeginChild("left pane", ImVec2(350, 0), true);
        auto visible_entities = this->game->registry.view<Spacial, Texture, ToRender>();
        for (const auto entity : visible_entities) {
            auto [spacial, texture] = visible_entities.get<Spacial, Texture>(entity);
            char label[128];
            if (this->game->registry.all_of<Name>(entity)) {
                auto name = this->game->registry.get<Name>(entity);
                sprintf(label, "%10d %s", (int)entity, name.name.c_str());
            } else {
                sprintf(label, "%10d %s", (int)entity, "<unnamed>");
            }
            if (ImGui::Selectable(label, this->selected_entity == entity)) {
                this->selected_entity = entity;
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("entity view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        if (this->selected_entity != entt::null && this->game->registry.all_of<Spacial, Texture>(this->selected_entity)) {
            auto [spacial, texture] = this->game->registry.get<Spacial, Texture>(this->selected_entity);
            if (this->game->registry.all_of<Name>(this->selected_entity)) {
                auto name = this->game->registry.get<Name>(this->selected_entity);
                ImGui::Text("%10d %s", (int)this->selected_entity, name.name.c_str());
            } else {
                ImGui::Text("%10d %s", (int)this->selected_entity, "<unnamed>");
            }
            ImGui::Separator();

            using namespace entt::literals;
            Camera& camera = this->game->registry.ctx().at<Camera&>("world_camera"_hs);

            const float max_texture_dimension = std::max(texture.frame_data->size.x, texture.frame_data->size.y);
            float edge_spacing = 1.2f * max_texture_dimension * camera.getZoom();
            const float preview_start_x = spacial.position.x * camera.getZoom() - // Position of entity in pixels
                (camera.getPosition().x * camera.getZoom() - ((float)globals::SCREEN_WIDTH/2.0f)) - // Position of edge of camera in pixels
                edge_spacing;
            const float preview_start_y = -spacial.position.y * camera.getZoom() + 
                (camera.getPosition().y * camera.getZoom() - (float)globals::SCREEN_HEIGHT/2.0f) + 
                edge_spacing;
            const float preview_end_x = (spacial.position.x + max_texture_dimension) * camera.getZoom() - 
                (camera.getPosition().x * camera.getZoom() - ((float)globals::SCREEN_WIDTH/2.0f)) +
                edge_spacing;
            const float preview_end_y = (-spacial.position.y - max_texture_dimension) * camera.getZoom() + 
                (camera.getPosition().y * camera.getZoom() - ((float)globals::SCREEN_HEIGHT/2.0f)) -
                edge_spacing;
            ImGui::Image(
                (void*)(intptr_t)this->game->screen_texture,
                {100, 100},
                {
                    preview_start_x / (float)globals::SCREEN_WIDTH,
                    preview_start_y / (float)globals::SCREEN_HEIGHT
                },
                {
                    preview_end_x / (float)globals::SCREEN_WIDTH,
                    preview_end_y / (float)globals::SCREEN_HEIGHT
                }
            );
            ImGui::SameLine();

            const float texture_scale = 100.0f / (float)((texture.frame_data->size.x > texture.frame_data->size.y) ? 
                texture.frame_data->size.x : texture.frame_data->size.y);
            const float texture_start_x = (float)texture.frame_data->position.x;
            const float texture_start_y = (float)texture.frame_data->position.y;
            const float texture_end_x = (float)texture.frame_data->position.x + (float)texture.frame_data->size.x;
            const float texture_end_y = (float)texture.frame_data->position.y + (float)texture.frame_data->size.y;
            ImGui::Image(
                (void*)(intptr_t)this->game->texture_atlas.gl_texture_id,
                {texture.frame_data->size.x * texture_scale, texture.frame_data->size.y * texture_scale},
                {
                    texture_start_x / (float)this->game->texture_atlas.width,
                    texture_start_y / (float)this->game->texture_atlas.height
                },
                {
                    texture_end_x / (float)this->game->texture_atlas.width,
                    texture_end_y / (float)this->game->texture_atlas.height
                }
            );
            ImGui::Separator();
            this->game->registry.patch<Spacial>(this->selected_entity, [this](auto& s) {
                ImGui::Text("Spacial");
                ImGui::Indent();
                ImGui::Text("Position: \n\tx: %.2f \n\ty: %.2f \n\tz: %.2f", s.position.x, s.position.y, s.position.z);

                ImGui::TextUnformatted("Rotation");
                ImGui::SliderFloat3("##1", &s.rotation[0], -6.0f, 6.0f);
                ImGui::SameLine();
                if (ImGui::Button("Reset##1")) {
                    s.rotation = glm::vec3(0, 0, 0);
                }

                ImGui::TextUnformatted("Scale");
                ImGui::SliderFloat2("##2", &s.scale[0], 0.01f, 10.0f);
                ImGui::SameLine();
                if (ImGui::Button("Reset##2")) {
                    s.scale = glm::vec3(1, 1, 1);
                }

                ImGui::Text("Dimensions: \n\tx: %.2f \n\ty: %.2f", s.dimensions.x, s.dimensions.y);
                const char* direction_string[]{"", "UP", "DOWN", "LEFT", "RIGHT"};
                ImGui::Text("Direction: %s", direction_string[s.direction]);
                ImGui::Unindent();
            });
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void DebugWindow::showShaderViewer() {
    ImGui::SetNextWindowSize({800, 500}, 0);
    if (ImGui::Begin("Shader Viewer", &this->open_shader_viewer, 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    )) {
        ImGui::BeginChild("left pane", ImVec2(300, 220), true);
        for (auto& [shader_name, shader_program] : this->game->shader_manager.shaders) {
            if (ImGui::Selectable(shader_name.c_str(), this->selected_shader == shader_program)) {
                this->selected_shader = shader_program;
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("action window", ImVec2(0, 220)); 
        if (this->selected_shader != NULL) {
            ImGui::Spacing();
            if (ImGui::Button("Recompile")) {
                this->selected_shader->recompile();
            }
        }
        ImGui::EndChild();
        ImGui::BeginChild("error view", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar); 
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); 
        for (auto log : this->game->shader_manager.logs) {
            const char* message = log.c_str();
            ImVec4 color;
            bool has_color = false;
            if (strstr(message, "[error]")) { 
                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; 
            } else if (strstr(message, "[status]")) { 
                color = ImVec4(1.0f, 0.7f, 0.5f, 1.0f); has_color = true; 
            }
            if (has_color) {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            }
            ImGui::TextUnformatted(message);
            if (has_color) {
                ImGui::PopStyleColor();
            }
        }

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
        // Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (this->scroll_to_botton || (this->auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {   
            ImGui::SetScrollHereY(1.0f);
        }
        this->scroll_to_botton = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
    }
    ImGui::End();
}