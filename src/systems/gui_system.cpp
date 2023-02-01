#include "gui_system.h"

GUISystem::GUISystem(entt::registry& registry) : System(registry) {}

void GUISystem::update() {
    this->updateFPSCounter();
}

void GUISystem::updateFPSCounter() {

    auto counters = this->registry.view<Text, FPSCounter>();

    for (auto& entity : counters) {

        auto& text = counters.get<Text>(entity);

        std::ostringstream ss;
        ss.precision(1);

        ss << std::setw(5) << std::fixed << this->registry.ctx().at<Clock&>().getSmoothedFPS();

        text.text = ss.str();
    }
}