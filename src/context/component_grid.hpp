#pragma once

#include <entt/entt.hpp>
#include <lightgrid/grid.hpp>

#include "spacial.hpp"

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
    ComponentGrid(entt::registry& registry);
    ComponentGrid(ComponentGrid&& component_grid) = default;
    ComponentGrid& operator=(ComponentGrid&& component_grid) = default;

    void init(int width, int height, int cell_size);
    void update();

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

    lightgrid::grid<entt::entity> grid;
    entt::observer observer;
    entt::registry& registry;
};

template<typename Component>
ComponentGrid<Component>::ComponentGrid(entt::registry& registry) : registry{ registry } { 
    this->observer.connect(registry, entt::collector.update<Spacial>().where<Component>());
    registry.on_construct<Component>().template connect<&ComponentGrid<Component>::observeConstruct>(this);
    registry.on_destroy<Component>().template connect<&ComponentGrid<Component>::observeDestroy>(this);
}

template<typename Component>
void ComponentGrid<Component>::init(int width, int height, int cell_size) {
    this->grid.init(width, height, cell_size);
}

template<typename Component>
void ComponentGrid<Component>::update() {

    // Update the grid for the related component
    this->observer.each([&, this](auto entity){

        assert((this->registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

        // Remove the old data from the component grid
        auto& grid_data = this->registry.get<GridData<Component>>(entity);
        this->grid.remove(grid_data.node, grid_data.bounds);

        // Add the new data
        auto& spacial = this->registry.get<Spacial>(entity);
        grid_data.bounds = (struct lightgrid::bounds) {
            static_cast<int>(spacial.pos.x), static_cast<int>(spacial.pos.y), 
            static_cast<int>(spacial.dim.x), static_cast<int>(spacial.dim.y) };
        grid_data.node = this->grid.insert(entity, grid_data.bounds);
    });
    
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
    
    
    assert((registry.all_of<Spacial>(entity) && "Entity missing Spacial component"));

    auto& spacial = registry.get<Spacial>(entity);
    lightgrid::bounds bounds{
        static_cast<int>(spacial.pos.x), static_cast<int>(spacial.pos.y), 
        static_cast<int>(spacial.dim.x), static_cast<int>(spacial.dim.y) };

    int element_node = this->grid.insert(entity, bounds);

    registry.emplace<GridData<Component>>(entity, bounds, element_node);
}

template<typename Component>
void ComponentGrid<Component>::observeDestroy(entt::registry& registry, entt::entity entity) {
    assert((registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

    auto& grid_data = registry.get<GridData<Component>>(entity);
    this->grid.remove(grid_data.node, grid_data.bounds);
}