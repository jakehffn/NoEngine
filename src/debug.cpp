#include "debug.hpp"

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
        auto visible_entities = this->game->registry.view<Spacial, Texture, Name>();
        for (const auto entity : visible_entities) {
            auto [spacial, texture, name] = visible_entities.get<Spacial, Texture, Name>(entity);
            char label[128];
            sprintf(label, "%d %s", (int)entity, name.name.c_str());
            if (ImGui::Selectable(label, this->selected_entity == entity)) {
                this->selected_entity = entity;
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("entity view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        ImGui::Text("%d", (int)this->selected_entity);
        ImGui::Separator();

        if (this->game->registry.all_of<Spacial, Texture>(this->selected_entity)) {

            auto [spacial, texture] = this->game->registry.get<Spacial, Texture>(this->selected_entity);
            using namespace entt::literals;
            Camera& camera = this->game->registry.ctx().at<Camera&>("world_camera"_hs);
    
            ImGui::Image(
                (void*)(intptr_t)this->game->screen_texture,
                {100, 100},
                {
                    ((float)spacial.pos.x - ((float)camera.getPosition().x - ((float)render_c::SCREEN_WIDTH/2.0) / camera.getZoom())) / ((float)render_c::SCREEN_WIDTH / camera.getZoom()),
                    ((float)-spacial.pos.y + ((float)camera.getPosition().y - ((float)render_c::SCREEN_HEIGHT/2.0) / camera.getZoom())) / ((float)render_c::SCREEN_HEIGHT / camera.getZoom())
                },
                {
                    (((float)spacial.pos.x + 50) - ((float)camera.getPosition().x - ((float)render_c::SCREEN_WIDTH/2.0) / camera.getZoom())) / ((float)render_c::SCREEN_WIDTH / camera.getZoom()),
                    (((float)-spacial.pos.y - 50) + ((float)camera.getPosition().y - ((float)render_c::SCREEN_HEIGHT/2.0) / camera.getZoom())) / ((float)render_c::SCREEN_HEIGHT / camera.getZoom())
                }
            );
            ImGui::SameLine();
            const float texture_scale = 2;
            ImGui::Image(
                (void*)(intptr_t)this->game->texture_atlas.gl_texture_id,
                {texture.frame_data->size.x * texture_scale, texture.frame_data->size.y * texture_scale},
                {
                    (float)texture.frame_data->position.x / (float)this->game->texture_atlas.width,
                    (float)texture.frame_data->position.y / (float)this->game->texture_atlas.height
                },
                {
                    ((float)texture.frame_data->position.x + (float)texture.frame_data->size.x) / (float)this->game->texture_atlas.width,
                    ((float)texture.frame_data->position.y + (float)texture.frame_data->size.y) / (float)this->game->texture_atlas.height
                }
            );
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}