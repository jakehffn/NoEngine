#pragma once

#include <sstream>
#include <iomanip>

#include <entt\entt.hpp>

#include "system.hpp"

#include "text.hpp"
#include "fps_counter.hpp"

#include "clock.hpp"
#include "children.hpp"
#include "resource_loader.hpp"

#include "debug_timer.hpp"

class GuiSystem : public System {
public:
    GuiSystem(entt::registry& registry);

    void update() override;

private:
    void updateFPSCounter();
    void updateDialogs();

    void beginDialog(entt::registry& registry, entt::entity entity);
    void endDialog(entt::registry& registry, entt::entity entity);
};