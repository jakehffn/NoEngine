#pragma once

#include <entt\entt.hpp>

class System {
public:

    System(entt::registry& registry) : registry{ registry } {}
    
    virtual void update() = 0;

protected:
    entt::registry& registry;

};