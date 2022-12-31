#pragma once

#include <sstream>
#include <iomanip>

#include <entt\entt.hpp>

#include "System.h"

#include "Text.h"
#include "FPSCounter.h"

#include "Clock.h"

class GUISystem : public System {
public:
    GUISystem(entt::registry& registry);

    void update() override;

private:
    void updateFPSCounter();
};