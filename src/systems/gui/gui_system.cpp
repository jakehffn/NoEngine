#include "gui_system.hpp"

GuiSystem::GuiSystem(entt::registry& registry) : System(registry) {
    auto fps_counter = this->registry.create();
    this->registry.emplace<Spacial>(fps_counter, glm::vec3(-232,-125,0));
    this->registry.emplace<Text>(fps_counter);
    this->registry.emplace<FpsCounter>(fps_counter);
    this->registry.emplace<GuiElement>(fps_counter);
    this->registry.emplace<Persistent>(fps_counter);

    this->registry.on_construct<Dialog>().connect<GuiSystem::beginDialog>(this);
}

void GuiSystem::update() {
    DEBUG_TIMER(_, "GuiSystem::update");
    this->updateFPSCounter();
    this->updateDialogs();
}

void GuiSystem::updateFPSCounter() {
    auto& clock = this->registry.ctx().at<Clock&>();

    for (auto& entity : this->registry.view<Text, FpsCounter>()) {
        auto& fps_counter = this->registry.get<FpsCounter>(entity);
        fps_counter.timer -= clock.getDeltaTime();

        if (fps_counter.timer < 0) {
            fps_counter.timer = fps_counter.timer_reset;

            std::ostringstream ss;
            ss.precision(1);

            ss << std::setw(5) << std::fixed << this->registry.ctx().at<Clock&>().getSmoothedFPS();

            this->registry.patch<Text>(entity, [&ss](auto& text) {
                text.text = ss.str();
            });
        }

    }
}

void GuiSystem::updateDialogs() {
    auto& clock = this->registry.ctx().at<Clock&>();

    for (auto entity : this->registry.view<Dialog>()) {
        auto& dialog = this->registry.get<Dialog>(entity);

        auto next = dialog.current_step->next(registry, entity, clock.getDeltaTime());
        if (next != NULL) {
            dialog.current_step = next;
        }
    }
}

void GuiSystem::beginDialog(entt::registry& registry, entt::entity entity) {
    registry.emplace<Children>(entity);
    auto& dialog = registry.get<Dialog>(entity);
    dialog.current_step->begin(registry, entity);
}