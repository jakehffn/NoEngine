#pragma once

#include <entt\entt.hpp>

class System {
public:
    virtual void update(entt::registry&)=0;
};