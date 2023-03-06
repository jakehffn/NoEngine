#pragma once

#include <sstream>
#include <iomanip>

#include <entt\entt.hpp>

#include "system.hpp"

#include "text.hpp"
#include "fps_counter.hpp"

#include "clock.hpp"

class GUISystem : public System {
public:
    GUISystem(entt::registry& registry);

    void update() override;

private:
    void updateFPSCounter();
};