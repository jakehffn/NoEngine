#pragma once

#include <cassert>
#include <vector>
#include <set>
#include <algorithm>

template<typename C, typename T>
concept Insertable = requires(C& c, T t) {
    {c.insert(c.end(), std::forward<T>(t))};
};

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

// @brief Data-structure for spacial look-up.
// Divides 2D coordinates into cells, allowing for insertion and lookup for 
//      an arbitrary type T, based on position.
template<class T>
class Grid {
public:
    void init(int width, int height, int cell_size);
    void clear();
    int insert(T element, const Bounds& bounds);
    void remove(int element_node, const Bounds& bounds);
    void update(int element_node, const Bounds& old_bounds, const Bounds& new_bounds);

    template<template<typename Rtype> typename R, typename Rtype=T> 
    requires Insertable<R<Rtype>, Rtype>
    R<Rtype>& query(const Bounds& bounds, R<T>& results);
    
private:
    int elementInsert(T element);
    void elementRemove(int element_node);

    void cellInsert(int cell_node, int element_node);
    void cellRemove(int cell_node, int element_node);
    void cellQuery(int cell_node, int unused);

    void iterateBounds(int node, const Bounds& bounds, void (Grid::*func)(int, int));

    std::vector<T> elements;
    std::vector<Node> element_nodes;
    std::vector<Node> cell_nodes; // The first cells in this list will never change and will be accessed directly, acting as the 2D list of cells

    std::set<int> last_query;

    int free_element_nodes=-1; // singly linked-list of the free nodes
    int free_cell_nodes=-1; 

    int width;
    int height;
    int cell_size;
    int cell_row_size;
    int cell_column_size;
    int num_cells;
};

template<class T>
void Grid<T>::init(int width, int height, int cell_size) {

    this->width = width;
    this->height = height;
    this->cell_size = cell_size;
    this->cell_row_size = (width+cell_size-1)/cell_size;
    this->cell_column_size = (height+cell_size-1)/cell_size;

    this->clear();
}

template<class T>
void Grid<T>::clear() {

    this->elements.clear();
    this->element_nodes.clear();
    this->cell_nodes.clear();

    this->num_cells = this->cell_row_size * ((this->height+this->cell_size-1)/this->cell_size);
    this->cell_nodes.resize(num_cells);
}

template<class T>
int Grid<T>::insert(T element, const Bounds& bounds) {

    assert(this->cell_nodes.size() > 0 && "Insert attempted on uninitialized Grid");

    int new_element_node = this->elementInsert(element);
    this->iterateBounds(new_element_node, bounds, &(this->cellInsert));

    return new_element_node;
}

template<class T>
void Grid<T>::remove(int element_node, const Bounds& bounds) {

    assert(this->cell_nodes.size() > 0 && "Remove attempted on uninitialized Grid");

    this->iterateBounds(element_node, bounds, &(this->cellRemove));
    this->elementRemove(element_node);
}

template<class T>
void Grid<T>::update(int element_node, const Bounds& old_bounds, const Bounds& new_bounds) {

    // TODO: make a specialized function for updates
}

template<class T>
template<template<typename Rtype> typename R, typename Rtype> 
requires Insertable<R<Rtype>, Rtype>
R<Rtype>& Grid<T>::query(const Bounds& bounds, R<T>& results) {

    assert(this->cell_nodes.size() > 0 && "Query attempted on uninitialized Grid");

    this->last_query.clear();
    this->iterateBounds(-1, bounds, &(this->cellQuery));
    
    std::transform(last_query.begin(), last_query.end(), std::inserter(results, results.end()), 
        ([this](const auto& element) {
            return this->elements[this->element_nodes[element].element];
        })
    );

    return results;
}

template<class T>
int Grid<T>::elementInsert(T element) {

    int new_element_node;

    if (this->free_element_nodes != -1) {

        // Use the first item in the linked list and move the head to the next free node
        new_element_node = this->free_element_nodes;
        free_element_nodes = this->element_nodes[this->free_element_nodes].next;

        this->elements[element_nodes[new_element_node].element] = element;

    } else {

        // Create new element node and add reference to index into elements list
        new_element_node = this->element_nodes.size();
        this->element_nodes.emplace_back(this->elements.size());
        this->elements.push_back(element);
    }

    return new_element_node;
}

template<class T>
void Grid<T>::elementRemove(int element_node) {

    // Make the given element_node the head of the free_element_nodes list
    this->element_nodes[element_node].next = this->free_element_nodes;
    this->free_element_nodes = element_node;
}

template<class T>
void Grid<T>::cellInsert(int cell_node, int element_node) {

    if (this->free_cell_nodes != -1) {

        // Use element of free node as scratchpad for next free node
        this->cell_nodes[this->free_cell_nodes].element = this->cell_nodes[this->free_cell_nodes].next;

        // Move head of cell's linked list to the free node
        this->cell_nodes[this->free_cell_nodes].next = this->cell_nodes[cell_node].next;
        this->cell_nodes[cell_node].next = this->free_cell_nodes;

        // Move head of free nodes to the value in scratchpad and set head of cell to the element node
        this->free_cell_nodes = this->cell_nodes[this->free_cell_nodes].element;
        this->cell_nodes[this->cell_nodes[cell_node].next].element = element_node;

    } else {

        // Create new cell node and add reference to index into element_nodes list
        this->cell_nodes.emplace_back(element_node, this->cell_nodes[cell_node].next);
        this->cell_nodes[cell_node].next = this->cell_nodes.size() - 1;

    }
}

template<class T>
void Grid<T>::cellRemove(int cell_node, int element_node) {

    int previous_node{cell_node};
    int current_node{this->cell_nodes[cell_node].next};

    // Find the element_node in the cell_node's list
    while (this->cell_nodes[current_node].element != element_node) {
        previous_node = current_node;
        current_node = this->cell_nodes[current_node].next;
    }

    // Remove the cell_node containing element_node
    this->cell_nodes[previous_node].next = this->cell_nodes[current_node].next;
    // Make the currentNode the head of the free_cell_nodes list 
    this->cell_nodes[current_node].next = this->free_cell_nodes;
    this->free_cell_nodes = current_node;
}

template<class T>
void Grid<T>::cellQuery(int cell_node, int unused) {

    int current_node{this->cell_nodes[cell_node].next};

    while (current_node != -1) {
        assert(current_node < this->cell_nodes.size() && "current_node out of bounds");
        this->last_query.insert(this->cell_nodes[current_node].element);
        current_node = this->cell_nodes[current_node].next;
    }
}

template<class T>
void Grid<T>::iterateBounds(int node, const Bounds& bounds, void (Grid::*func)(int, int)) {

    // Snap to the edge if extending past the map boundaries
    // Expanded boundaries allow for issues with cell boundaries to be avoided
    const int expanded_bound{1};
    
    int x_cell_start = std::clamp(bounds.x/this->cell_size - expanded_bound, 
        0, this->cell_row_size);
    int y_cell_start = std::clamp(bounds.y/this->cell_size - expanded_bound, 
        0, this->cell_column_size);
    int x_cell_end = std::clamp(bounds.w/this->cell_size + x_cell_start + expanded_bound,
        0, this->cell_row_size);
    int y_cell_end = std::clamp(bounds.h/this->cell_size + y_cell_start + expanded_bound,
        0, this->cell_column_size);

    for (int xx{x_cell_start}; xx < x_cell_end; xx++) {
        for (int yy{y_cell_start}; yy < y_cell_end; yy++) {
            (this->*func)(yy*this->cell_row_size + xx, node);           
        }
    }
}