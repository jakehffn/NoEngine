#pragma once

#include <functional>

#include <entt/entt.hpp>
#include <lightgrid/grid.hpp>

#include "spacial.hpp"
#include "component_grid_ignore.hpp"
#include "debug_timer.hpp"

template<typename>
struct GridData {
    lightgrid::bounds bounds;
    int node;
};

// Grid wrapper for EnTT to insert, update, and remove components as their spacials update
// Note: the bounds used for the grid are the spacial bounds. 
// Bounding boxes of the collision component are not considered.
// This should probably be changed 
template<typename Component>
class ComponentGrid {
public:
    ComponentGrid(
        entt::registry& registry, 
        std::function<lightgrid::bounds (entt::registry&, entt::entity)> getBounds
    );
    ComponentGrid(ComponentGrid&& component_grid) = default;
    ComponentGrid& operator=(ComponentGrid&& component_grid) = default;

    void init(int width, int height, int cell_size);
    void update();
    void connect();
    void disconnect();

    template<template<typename Rtype> typename R, typename Rtype=entt::entity> 
    requires lightgrid::insertable<R<Rtype>, Rtype>
    R<Rtype>& query(const lightgrid::bounds& bounds, R<entt::entity>& results);

    template<template<typename Rtype> typename R, typename Rtype=entt::entity> 
    requires lightgrid::insertable<R<Rtype>, Rtype>
    R<Rtype>& query(const float x, const float y, const float w, const float h, R<entt::entity>& results);

    void clear();

private:
    void observeConstruct(entt::registry& registry, entt::entity entity);
    void observeDestroy(entt::registry& registry, entt::entity entity);

    std::function<lightgrid::bounds (entt::registry&, entt::entity)> getBounds;

    lightgrid::grid<entt::entity> grid;
    entt::observer observer;
    entt::registry& registry;

    bool is_initialized{false};
};

template<typename Component>
ComponentGrid<Component>::ComponentGrid(
    entt::registry& registry, 
    std::function<lightgrid::bounds (entt::registry&, entt::entity)> getBounds
) : registry{registry}, getBounds{getBounds} { 
    this->connect();
}

template<typename Component>
void ComponentGrid<Component>::init(int width, int height, int cell_size) {
    this->grid.init(width, height, cell_size);

    for (auto entity : this->registry.view<Component, Spacial>()) {
        this->observeConstruct(this->registry, entity);
    }

    this->is_initialized = true;
}

template<typename Component>
void ComponentGrid<Component>::update() {
    DEBUG_TIMER(_,"ComponentGrid::update");
    // Update the grid for the related component
    this->observer.each([&, this](auto entity){

        assert((this->registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

        // Remove the old data from the component grid
        auto& grid_data = this->registry.get<GridData<Component>>(entity);
        this->grid.remove(grid_data.node, grid_data.bounds);

        // Add the new data
        auto& spacial = this->registry.get<Spacial>(entity);
        grid_data.bounds = this->getBounds(this->registry, entity);
        grid_data.node = this->grid.insert(entity, grid_data.bounds);
    });
    
}

template<typename Component>
void ComponentGrid<Component>::connect() {
    this->observer.connect(registry, entt::collector.update<Spacial>().where<Component>(entt::exclude<ComponentGridIgnore>));
    registry.on_construct<Component>().template connect<&ComponentGrid<Component>::observeConstruct>(this);
    registry.on_destroy<Component>().template connect<&ComponentGrid<Component>::observeDestroy>(this);
}

template<typename Component>
void ComponentGrid<Component>::disconnect() {
    this->observer.disconnect();
    registry.on_construct<Component>().template disconnect<&ComponentGrid<Component>::observeConstruct>(this);
    registry.on_destroy<Component>().template disconnect<&ComponentGrid<Component>::observeDestroy>(this);
}

template<typename Component>
template<template<typename Rtype> typename R, typename Rtype> 
requires lightgrid::insertable<R<Rtype>, Rtype>
R<Rtype>& ComponentGrid<Component>::query(const lightgrid::bounds& bounds, R<entt::entity>& results) {
    return this->grid.query(bounds, results);
}

template<typename Component>
template<template<typename Rtype> typename R, typename Rtype> 
requires lightgrid::insertable<R<Rtype>, Rtype>
R<Rtype>& ComponentGrid<Component>::query(float x, float y, float w, float h, R<entt::entity>& results) {
    return this->query({
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(w),
        static_cast<int>(h)
    }, results);
}

template<typename Component>
void ComponentGrid<Component>::clear() {
    this->grid.clear();
}

template<typename Component>
void ComponentGrid<Component>::observeConstruct(entt::registry& registry, entt::entity entity) {
    if (registry.any_of<ComponentGridIgnore>(entity) || !this->is_initialized) {
        return;
    }
    
    assert((registry.all_of<Spacial>(entity) && "Entity missing Spacial component"));

    auto bounds = this->getBounds(registry, entity);

    int element_node = this->grid.insert(entity, bounds);
    registry.emplace<GridData<Component>>(entity, bounds, element_node);
    // Trigger a collision check on construction
    this->registry.patch<Spacial>(entity);
}

template<typename Component>
void ComponentGrid<Component>::observeDestroy(entt::registry& registry, entt::entity entity) {
    if (!registry.all_of<GridData<Component>>(entity)) {
        return;
    }

    assert((
        registry.all_of<Spacial, GridData<Component>>(entity) && 
        "Entity missing Spacial or GridData<T> component\n\tEnsure Grid component is destroyed before spacial"
    ));

    auto& grid_data = registry.get<GridData<Component>>(entity);
    this->grid.remove(grid_data.node, grid_data.bounds);
}