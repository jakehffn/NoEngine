#pragma once

#include <cassert>
#include <vector>
#include <set>
#include <type_traits>

#include <iostream>

#include <entt/entt.hpp>

#include "spacial.h"

struct Bounds {
    int x,y,w,h;
};

struct Node {
    Node() {};
    Node(int element) : element{ element } {};
    Node(int element, int next) : element{ element }, next{ next } {};
    // Index of element in element list
    int element=-1;
    // Either the index of the next element in the cell or the next element in the free list
    // -1 if the end of either list
    int next=-1; 
};

template<class T>
class Grid {
public:
    void init(int width, int height, int cellSize);
    void clear();
    int insert(T element, Bounds bounds);
    void remove(int elementNode, Bounds bounds);
    void update(int elementNode, Bounds oldBounds, Bounds newBounds);
    std::vector<T> query(Bounds bounds);

private:
    int elementInsert(T element);
    void elementRemove(int elementNode);

    void cellInsert(int cellNode, int elementNode);
    void cellRemove(int cellNode, int elementNode);
    void cellQuery(int cellNode, int unused);

    void iterateBounds(int node, Bounds bounds, void (Grid::*function)(int, int));

    std::vector<T> elements;
    std::vector<Node> elementNodes;
    std::vector<Node> cellNodes; // The first cells in this list will never change and will be accessed directly, acting as the 2D list of cells

    std::set<int> lastQuery;

    int freeElementNodes=-1; // singly linked-list of the free nodes
    int freeCellNodes=-1; 

    int width;
    int height;
    int cellSize;
    int cellRowLength;
};

template<class T>
void Grid<T>::init(int width, int height, int cellSize) {

    this->width = width;
    this->height = height;
    this->cellSize = cellSize;
    this->cellRowLength = (width+cellSize-1)/cellSize;

    this->clear();
}

template<class T>
void Grid<T>::clear() {

    this->elements.clear();
    this->elementNodes.clear();
    this->cellNodes.clear();

    int numCells = this->cellRowLength * ((this->height+this->cellSize-1)/this->cellSize);
    this->cellNodes.resize(numCells);

    std::cout << "Initialized with " << cellNodes.size() << " elements" << "\n";
}

template<class T>
int Grid<T>::insert(T element, Bounds bounds) {

    assert(this->cellNodes.size() > 0 && "Insert attempted on uninitialized Grid");

    int newElementNode = this->elementInsert(element);
    this->iterateBounds(newElementNode, bounds, &(this->cellInsert));

    return newElementNode;
}

template<class T>
void Grid<T>::remove(int elementNode, Bounds bounds) {

    assert(this->cellNodes.size() > 0 && "Remove attempted on uninitialized Grid");

    this->iterateBounds(elementNode, bounds, &(this->cellRemove));
    this->elementRemove(elementNode);
}

template<class T>
void Grid<T>::update(int elementNode, Bounds oldBounds, Bounds newBounds) {

    // TODO: make a specialized function for updates
}

template<class T>
std::vector<T> Grid<T>::query(Bounds bounds) {

    assert(this->cellNodes.size() > 0 && "Query attempted on uninitialized Grid");

    this->lastQuery.clear();
    this->iterateBounds(-1, bounds, &(this->cellQuery));

    std::vector<T> results;
    results.reserve(this->lastQuery.size());
    
    for (auto element : this->lastQuery) {
        results.push_back(this->elements[this->elementNodes[element].element]);
    }

    return results;
}

template<class T>
int Grid<T>::elementInsert(T element) {

    int newElementNode;

    if (this->freeElementNodes != -1) {

        // Use the first item in the linked list and move the head to the next free node
        newElementNode = this->freeElementNodes;
        freeElementNodes = this->elementNodes[this->freeElementNodes].next;

        this->elements[elementNodes[newElementNode].element] = element;

    } else {

        // Create new element node and add reference to index into elements list
        newElementNode = this->elementNodes.size();
        this->elementNodes.emplace_back(this->elements.size());
        this->elements.push_back(element);
    }

    return newElementNode;
}

template<class T>
void Grid<T>::elementRemove(int elementNode) {

    // Make the given elementNode the head of the freeElementNodes list
    this->elementNodes[elementNode].next = this->freeElementNodes;
    this->freeElementNodes = elementNode;
}

template<class T>
void Grid<T>::cellInsert(int cellNode, int elementNode) {

    if (this->freeCellNodes != -1) {

        // Use element of free node as scratchpad for next free node
        this->cellNodes[this->freeCellNodes].element = this->cellNodes[this->freeCellNodes].next;

        // Move head of cell's linked list to the free node
        this->cellNodes[this->freeCellNodes].next = this->cellNodes[cellNode].next;
        this->cellNodes[cellNode].next = this->freeCellNodes;

        // Move head of free nodes to the value in scratchpad and set head of cell to the element node
        this->freeCellNodes = this->cellNodes[this->freeCellNodes].element;
        this->cellNodes[this->cellNodes[cellNode].next].element = elementNode;

    } else {

        // Create new cell node and add reference to index into elementNodes list
        this->cellNodes.emplace_back(elementNode, this->cellNodes[cellNode].next);
        this->cellNodes[cellNode].next = this->cellNodes.size() - 1;

    }
}

template<class T>
void Grid<T>::cellRemove(int cellNode, int elementNode) {

    int previousNode = cellNode;
    int currentNode = this->cellNodes[cellNode].next;

    // Find the elementNode in the cellNode's list
    while (this->cellNodes[currentNode].element != elementNode) {
        previousNode = currentNode;
        currentNode = this->cellNodes[currentNode].next;
    }

    // Remove the cellNode containing elementNode
    this->cellNodes[previousNode].next = this->cellNodes[currentNode].next;
    // Make the currentNode the head of the freeCellNodes list 
    this->cellNodes[currentNode].next = this->freeCellNodes;
    this->freeCellNodes = currentNode;
}

template<class T>
void Grid<T>::cellQuery(int cellNode, int unused) {

    int currentNode = this->cellNodes[cellNode].next;

    while (currentNode != -1) {
        this->lastQuery.insert(this->cellNodes[currentNode].element);
        currentNode = this->cellNodes[currentNode].next;
    }
}

template<class T>
void Grid<T>::iterateBounds(int node, Bounds bounds, void (Grid::*function)(int, int)) {

    bounds.w++; // Add one to avoid entities close to border being missed because of conversion to int
    bounds.h++;

    assert((bounds.x >= 0 && bounds.y >= 0 && bounds.x+bounds.w <= this->width && bounds.y+bounds.h <= this->height) 
        && "Attempted out of bounds operation");

    int xCellStart = bounds.x/this->cellSize;
    int yCellStart = bounds.y/this->cellSize;

    int xCellEnd = (bounds.w+this->cellSize-1)/this->cellSize + xCellStart;
    int yCellEnd = (bounds.h+this->cellSize-1)/this->cellSize + yCellStart;

    for (int xx{xCellStart}; xx < xCellEnd; xx++) {

        for (int yy{yCellStart}; yy < yCellEnd; yy++) {

            (this->*function)(yy*this->cellRowLength + xx, node);           
        }
    }
}

// Very cool solution given from https://stackoverflow.com/questions/26169198/how-to-get-the-index-of-a-type-in-a-variadic-type-pack
// Since no information is given along with the answer, I'll explain it here: 
// This template will be evaluated at compile time, giving the index of
//      the first type in the list of types following.
// This works by having partial template specializations for the case where the types are the same and the case where they are different.
//      Both of these specializations inherit from std::integral_constant which has a static constexpr member, allong for the entire expression to 
//      be evaluated at compile time. 

// @brief Evaluates (at compile time) the index of T in Ts as a static member
template<typename T, typename... Ts>
struct Index;

template<typename T, typename... Ts>
struct Index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

template<typename T, typename U, typename... Ts>
struct Index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + Index<T, Ts...>::value> {};

// Evaluates (at compile time) the index of T in Ts
template<typename T, typename... Ts>
constexpr std::size_t Index_v = Index<T, Ts...>::value;

template<typename>
struct GridData {
    Bounds bounds;
    int node;
};

template<typename... Components>
class ComponentGrid {
public:
    ComponentGrid(entt::registry& registry);
    void init(int width, int height, int cellSize);
    template<typename Component>
    void update(entt::registry& registry);
    void update(entt::registry& registry);
    template<typename Component>
    std::vector<entt::entity> query(Bounds bounds);
    template<typename Component>
    void clear();

private:
    template<typename Component>
    void observeConstruct(entt::registry& registry, entt::entity entity);
    template<typename Component>
    void observeDestroy(entt::registry& registry, entt::entity entity);

    std::vector<Grid<entt::entity>> grids;
    std::vector<entt::observer> observers;
};

template<typename... Components>
ComponentGrid<Components...>::ComponentGrid(entt::registry& registry) : 
    observers{ std::vector<entt::observer>(sizeof...(Components)) },
    grids{ std::vector<Grid<entt::entity>>(sizeof...(Components)) } {
        
        (this->observers[Index_v<Components,Components...>].connect(registry, entt::collector.update<Spacial>().where<Components>()),...);
        ((registry.on_construct<Components>().template connect<&ComponentGrid<Components...>::observeConstruct<Components>>(this)),...);
        ((registry.on_destroy<Components>().template connect<&ComponentGrid<Components...>::observeDestroy<Components>>(this)),...);
}

template<typename... Components>
void ComponentGrid<Components...>::init(int width, int height, int cellSize) {

    for(auto& grid : this->grids) {
        grid.init(width, height, cellSize);
    }
}

template<typename... Components>
template<typename Component>
void ComponentGrid<Components...>::update(entt::registry& registry) {

    assert((std::is_same_v<Component, Components> || ...));
    this->observers[Index_v<Component, Components...>].each([&, registry, this](auto entity){

        assert((registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

        auto& gridData = registry.get<GridData<Component>>(entity);
        this->grid.remove(gridData.node, gridData.bounds);

        auto& spacial = registry.get<Spacial>(entity);
        gridData.elementNode = this->grid.insert(entity, gridData.bounds);
    });
    
}

template<typename... Components>
void ComponentGrid<Components...>::update(entt::registry& registry) {

    (this->update<Components>(registry),...);
}

template<typename... Components>
template<typename Component>
std::vector<entt::entity> ComponentGrid<Components...>::query(Bounds bounds) {

    assert((std::is_same_v<Component, Components> || ...));

    return this->grids[Index_v<Component, Components...>].query(bounds);
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
    Bounds bounds{spacial.pos.x, spacial.pos.y, spacial.dim.x, spacial.dim.y};
    int elementNode = this->grids[Index_v<Component, Components...>].insert(entity, bounds);

    registry.emplace<GridData<Component>>(entity, bounds, elementNode);
}

template<typename... Components>
template<typename Component>
void ComponentGrid<Components...>::observeDestroy(entt::registry& registry, entt::entity entity) {

    assert((std::is_same_v<Component, Components> || ...));
    assert((registry.all_of<Spacial, GridData<Component>>(entity) && "Entity missing Spacial or GridData<T> component"));

    auto& gridData = registry.get<GridData<Component>>(entity);
    this->grids[Index_v<Component, Components...>].remove(gridData.node, gridData.bounds);
}