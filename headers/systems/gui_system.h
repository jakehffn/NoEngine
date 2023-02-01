#pragma once

#include <sstream>
#include <iomanip>

#include <entt\entt.hpp>

#include "system.h"

#include "text.h"
#include "fps_counter.h"

#include "clock.h"

class GUISystem : public System {
public:
    GUISystem(entt::registry& registry);

    void update() override;

private:
    void updateFPSCounter();
};