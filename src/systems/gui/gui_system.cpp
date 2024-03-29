#include "gui_system.hpp"

GuiSystem::GuiSystem(entt::registry& registry) : System(registry) {
    auto fps_counter = this->registry.create();
    this->registry.emplace<Spacial>(fps_counter, glm::vec3(-232,-125,0));
    this->registry.emplace<Text>(fps_counter);
    this->registry.emplace<FpsCounter>(fps_counter);
    this->registry.emplace<GuiElement>(fps_counter);
    this->registry.emplace<Persistent>(fps_counter);

    this->registry.on_construct<Dialog>().connect<GuiSystem::beginDialog>(this);
    this->registry.on_destroy<Dialog>().connect<GuiSystem::endDialog>(this);
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
                std::string tmp = ss.str();
                text.text = std::u32string(tmp.begin(), tmp.end());
            });
        }

    }
}

void GuiSystem::updateDialogs() {
    auto& clock = this->registry.ctx().at<Clock&>();

    for (auto entity : this->registry.view<Dialog>()) {
        auto& dialog = this->registry.get<Dialog>(entity);

        if (!dialog.current_step->step(
            registry, 
            dialog.current_line_number, 
            entity, 
            clock.getDeltaTime())
        ) {
            auto next = dialog.current_step->next();
            if (next != NULL) {
                dialog.current_step = next;
                dialog.current_step->begin(
                    registry, 
                    dialog.current_line_number, 
                    entity
                );
            }
        }
    }
}

void GuiSystem::beginDialog(entt::registry& registry, entt::entity entity) {
    registry.emplace<Children>(entity);
    auto& dialog = registry.get<Dialog>(entity);
    dialog.current_step->begin(registry, dialog.current_line_number, entity);

    if (dialog.prevent_interaction) {
        auto& input = registry.ctx().at<Input&>();
        input.disableInteraction();
    }
}

void GuiSystem::endDialog(entt::registry& registry, entt::entity entity) {
    auto& dialog = registry.get<Dialog>(entity);
    if (dialog.prevent_interaction) {
        auto& input = registry.ctx().at<Input&>();
        input.enableInteraction();
    }
}