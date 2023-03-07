#pragma once

#include <entt/entt.hpp>
#include <lightgrid/grid.hpp>

#include "spacial.hpp"


// @brief Evaluates the index of T in Ts as a static member
template<typename T, typename... Ts>
struct Index;

template<typename T, typename... Ts>
struct Index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

template<typename T, typename U, typename... Ts>
struct Index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + Index<T, Ts...>::value> {};

// Evaluates the index of T in Ts
template<typename T, typename... Ts>
constexpr std::size_t Index_v = Index<T, Ts...>::value;

template<typename>
struct GridData {
    lightgrid::bounds bounds;
    int node;
};

// Grid wrapper for EnTT to insert, update, and remove components as their spacials update
// Note: the bounds used for the grid are the spacial bounds. 
// Bounding boxes of the collision component are not considered.
template<typename... Components>
class ComponentGrid {
public:
    ComponentGrid(entt::registry& registry);
    ComponentGrid(ComponentGrid&& component_grid) = default;
    ComponentGrid& operator=(ComponentGrid&& component_grid) = default;

    void init(int width, int height, int cell_size);
    template<typename Component>
    void update();
    void update();

    template<typename Component, template<typename Rtype> typename R, typename Rtype=entt::entity> 
    requires lightgrid::insertable<R<Rtype>, Rtype>
    R<Rtype>& query(const lightgrid::bounds& bounds, R<entt::entity>& results);

    template<typename Component, template<typename Rtype> typename R, typename Rtype=entt::entity> 
    requires lightgrid::insertable<R<Rtype>, Rtype>
    R<Rtype>& query(const float x, const float y, const float w, const float h, R<entt::entity>& results);


    template<typename Component>
    void clear();

private:
    template<typename Component>
    void observeConstruct(entt::registry& registry, entt::entity entity);
    template<typename Component>
    void observeDestroy(entt::registry& registry, entt::entity entity);

    std::vector<lightgrid::grid<entt::entity>> grids;
    std::vector<entt::observer> observers;
    entt::registry& registry;
};

template<typename... Components>
ComponentGrid<Components...>::ComponentGrid(entt::registry& registry) : 
    observers{ std::vector<entt::observer>(sizeof...(Components)) },
    grids{ std::vector<lightgrid::grid<entt::entity>>(sizeof...(Components)) }, 
    registry{ registry } {
        
        (this->observers[Index_v<Components,Components...>].connect(registry, entt::collector.update<Spacial>().where<Components>()),...);
        ((registry.on_construct<Components>().template connect<&ComponentGrid<Components...>::observeConstruct<Components>>(this)),...);
        ((registry.on_destroy<Components>().template connect<&ComponentGrid<Components...>::observeDestroy<Components>>(this)),...);
}

template<typename... Components>
void ComponentGrid<Components...>::init(int width, int height, int cell_size) {

    for(auto& grid : this->grids) {
        grid.init(width, height, cell_size);
    }
}

template<typename... Components>
template<typename Component>
void ComponentGrid<Components...>::update() {

    assert((std::is_same_v<Component, Components> || ...) && "Component not in Components");

    // Update the grid for the related component
    this->observers[Index_v<Component, Components...>].each([&, this](auto entity){

        assert((this->registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

        // Remove the old data from the component grid
        auto& grid_data = this->registry.get<GridData<Component>>(entity);
        this->grids[Index_v<Component, Components...>].remove(grid_data.node, grid_data.bounds);

        // Add the new data
        auto& spacial = this->registry.get<Spacial>(entity);
        grid_data.bounds = (struct lightgrid::bounds) {
            static_cast<int>(spacial.pos.x), static_cast<int>(spacial.pos.y), 
            static_cast<int>(spacial.dim.x), static_cast<int>(spacial.dim.y) };
        grid_data.node = this->grids[Index_v<Component, Components...>].insert(entity, grid_data.bounds);
    });
    
}

template<typename... Components>
void ComponentGrid<Components...>::update() {

    (this->update<Components>(),...);
}

template<typename... Components>
template<typename Component, template<typename Rtype> typename R, typename Rtype> 
requires lightgrid::insertable<R<Rtype>, Rtype>
R<Rtype>& ComponentGrid<Components...>::query(const lightgrid::bounds& bounds, R<entt::entity>& results) {

    assert((std::is_same_v<Component, Components> || ...));

    return this->grids[Index_v<Component, Components...>].query(bounds, results);
}

template<typename... Components>
template<typename Component, template<typename Rtype> typename R, typename Rtype> 
requires lightgrid::insertable<R<Rtype>, Rtype>
R<Rtype>& ComponentGrid<Components...>::query(float x, float y, float w, float h, R<entt::entity>& results) {

    assert((std::is_same_v<Component, Components> || ...));

    return this->query<Component>({
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(w),
        static_cast<int>(h)
    }, results);
}

template<typename... Components>
template<typename Component>
void ComponentGrid<Components...>::clear() {

    assert((std::is_same_v<Component, Components> || ...));

    this->grids[Index_v<Component, Components...>].clear();
}

template<typename... Components>
template<typename Component>
void ComponentGrid<Components...>::observeConstruct(entt::registry& registry, entt::entity entity) {

    assert((std::is_same_v<Component, Components> || ...));
    assert((registry.all_of<Spacial>(entity) && "Entity missing Spacial component"));

    auto& spacial = registry.get<Spacial>(entity);
    lightgrid::bounds bounds{
        static_cast<int>(spacial.pos.x), static_cast<int>(spacial.pos.y), 
        static_cast<int>(spacial.dim.x), static_cast<int>(spacial.dim.y) };

    int element_node = this->grids[Index_v<Component, Components...>].insert(entity, bounds);

    registry.emplace<GridData<Component>>(entity, bounds, element_node);
}

template<typename... Components>
template<typename Component>
void ComponentGrid<Components...>::observeDestroy(entt::registry& registry, entt::entity entity) {

    assert((std::is_same_v<Component, Components> || ...));
    assert((registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

    auto& grid_data = registry.get<GridData<Component>>(entity);
    this->grids[Index_v<Component, Components...>].remove(grid_data.node, grid_data.bounds);
}