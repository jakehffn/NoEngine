#include "debug.hpp"
#include "debug_timer.hpp"

DebugWindow::DebugWindow(Game* game) : game{game} {}

void DebugWindow::show() {
    if (ImGui::Begin("Debug", NULL, 
        ImGuiWindowFlags_MenuBar
    )) {
        this->showMenuBar();
        if (this->openTextureAtlas) {
            this->showTextureAtlas();
        }
        if (this->openEntityViewer) {
            this->showEntityViewer();
        }
        if (this->openShaderViewer) {
            this->showShaderViewer();
        }
    }
    ImGui::End();
}

void DebugWindow::showMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{8, 8});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{6, 6});
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("View", true)) {
            ImGui::MenuItem("Texture Atlas", "", &this->openTextureAtlas, true);
            ImGui::MenuItem("Entity Viewer", "", &this->openEntityViewer, true);
            ImGui::MenuItem("Timer Window", "", &DebugTimer::open_timers_window, true);
            ImGui::MenuItem("Shader Viewer", "", &this->openShaderViewer, true);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar(2);
}

void DebugWindow::showTextureAtlas() {
    ImGui::SetNextWindowSize({0, 0}, 0);
    if (ImGui::Begin("Texture Atlas", &this->openTextureAtlas, 
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
    ImGui::SetNextWindowSize({500, 440}, 0);
    if (ImGui::Begin("Entity Viewer", &this->openEntityViewer, 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    )) {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);
        auto visible_entities = this->game->registry.view<Spacial, Texture, ToRender>();
        for (const auto entity : visible_entities) {
            auto [spacial, texture] = visible_entities.get<Spacial, Texture>(entity);
            char label[128];
            if (this->game->registry.all_of<Name>(entity)) {
                auto name = this->game->registry.get<Name>(entity);
                sprintf(label, "%5d %s", (int)entity, name.name.c_str());
            } else {
                sprintf(label, "%5d %s", (int)entity, "<unnamed>");
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
                ImGui::Text("%5d %s", (int)this->selected_entity, name.name.c_str());
            } else {
                ImGui::Text("%5d %s", (int)this->selected_entity, "<unnamed>");
            }
            ImGui::Separator();

            using namespace entt::literals;
            Camera& camera = this->game->registry.ctx().at<Camera&>("world_camera"_hs);

            const float max_texture_dimension = std::max(texture.frame_data->size.x, texture.frame_data->size.y);
            float edge_spacing = 1.2f * max_texture_dimension * camera.getZoom();
            const float preview_start_x = spacial.pos.x * camera.getZoom() - // Position of entity in pixels
                (camera.getPosition().x * camera.getZoom() - ((float)constant::SCREEN_WIDTH/2.0f)) - // Position of edge of camera in pixels
                edge_spacing;
            const float preview_start_y = -spacial.pos.y * camera.getZoom() + 
                (camera.getPosition().y * camera.getZoom() - (float)constant::SCREEN_HEIGHT/2.0f) + 
                edge_spacing;
            const float preview_end_x = (spacial.pos.x + max_texture_dimension) * camera.getZoom() - 
                (camera.getPosition().x * camera.getZoom() - ((float)constant::SCREEN_WIDTH/2.0f)) +
                edge_spacing;
            const float preview_end_y = (-spacial.pos.y - max_texture_dimension) * camera.getZoom() + 
                (camera.getPosition().y * camera.getZoom() - ((float)constant::SCREEN_HEIGHT/2.0f)) -
                edge_spacing;
            ImGui::Image(
                (void*)(intptr_t)this->game->screen_texture,
                {100, 100},
                {
                    preview_start_x / (float)constant::SCREEN_WIDTH,
                    preview_start_y / (float)constant::SCREEN_HEIGHT
                },
                {
                    preview_end_x / (float)constant::SCREEN_WIDTH,
                    preview_end_y / (float)constant::SCREEN_HEIGHT
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
            ImGui::Text("Spacial");
            ImGui::Indent();
            ImGui::Text("Position: \n\tx: %.2f \n\ty: %.2f \n\tz: %.2f", spacial.pos.x, spacial.pos.y, spacial.pos.z);
            ImGui::Text("Rotation: \n\tx: %.2f \n\ty: %.2f \n\tz: %.2f", spacial.rot.x, spacial.rot.y, spacial.rot.z);
            ImGui::Text("Scale: \n\tx: %.2f \n\ty: %.2f \n\tz: %.2f", spacial.scale.x, spacial.scale.y, spacial.scale.z);
            ImGui::Text("Dimensions: \n\tx: %.2f \n\ty: %.2f", spacial.dim.x, spacial.dim.y);
            const char* direction_string[]{"", "UP", "DOWN", "LEFT", "RIGHT"};
            ImGui::Text("Direction: %s", direction_string[spacial.direction]);
            ImGui::Unindent();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void DebugWindow::showShaderViewer() {
    ImGui::SetNextWindowSize({500, 440}, 0);
    if (ImGui::Begin("Shader Viewer", &this->openShaderViewer, 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize
    )) {
        ImGui::BeginChild("left pane", ImVec2(250, 0), true);
        for (auto& [shader_name, shader_program] : this->game->shader_manager.shaders) {
            if (ImGui::Selectable(shader_name.c_str(), this->selected_shader == shader_program)) {
                this->selected_shader = shader_program;
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("error view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        if (this->selected_shader != NULL) {
            ImGui::Spacing();
            if (ImGui::Button("Recompile")) {
                this->selected_shader->recompile();
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}