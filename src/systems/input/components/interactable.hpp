#pragma once

#include <functional>

struct Interactable {
    std::function<void(entt::registry&, entt::entity)> action;
};