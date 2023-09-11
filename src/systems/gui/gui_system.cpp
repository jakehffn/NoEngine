#include "gui_system.hpp"

GuiSystem::GuiSystem(entt::registry& registry) : System(registry) {}

void GuiSystem::update() {
    DEBUG_TIMER(_, "GuiSystem::update");
    this->updateFPSCounter();
}

void GuiSystem::updateFPSCounter() {

    auto counters = this->registry.view<Text, FpsCounter>();

    for (auto& entity : counters) {

        auto& text = counters.get<Text>(entity);

        std::ostringstream ss;
        ss.precision(1);

        ss << std::setw(5) << std::fixed << this->registry.ctx().at<Clock&>().getSmoothedFPS();

        text.text = ss.str();
    }
}