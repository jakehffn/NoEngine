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
    }
    ImGui::End();
}

void DebugWindow::showMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{8, 8});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{6, 6});
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("View", true)) {
            ImGui::MenuItem("Texture Atlas", "", &this->openTextureAtlas, true);
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