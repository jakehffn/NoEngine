#pragma once

#include <cassert>

#include <vector>
#include <set>

#include <entt/entt.hpp>

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

template <class T>
class Grid {

public:

    void init(int width, int height, int cellSize);
    void clear();
    int insert(T element, int x, int y, int w, int h);
    void remove(int elementNode, int x, int y, int w, int h);
    void update(int elementNode, int x1, int y1, int x2, int y2, int w, int h);
    std::vector<T> query(int x, int y, int w, int h);

private:

    int elementInsert(T element);
    void elementRemove(int elementNode);

    void cellInsert(int cellNode, int elementNode);
    void cellRemove(int cellNode, int elementNode);
    void cellQuery(int cellNode, int unused);

    void iterateBounds(int node, int x, int y, int w, int h, void (Grid::*function)(int, int));

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

template <class T>
void Grid<T>::init(int width, int height, int cellSize) {

    this->width = width;
    this->height = height;
    this->cellSize = cellSize;
    this->cellRowLength = (width+cellSize-1)/cellSize;

    int numCells = this->cellRowLength * ((height+cellSize-1)/cellSize);
    this->cellNodes.resize(numCells);
}

template <class T>
void Grid<T>::clear() {

    this->elements.clear();
    this->elementNodes.clear();
    this->cellNodes.clear();

    int numCells = this->cellRowLength * ((this->height+this->cellSize-1)/this->cellSize);
    this->cellNodes.resize(numCells);
}

template <class T>
int Grid<T>::insert(T element, int x, int y, int w, int h) {

    int newElementNode = this->elementInsert(element);
    this->iterateBounds(newElementNode, x, y, w, h, &(this->cellInsert));

    return newElementNode;
}

template <class T>
void Grid<T>::remove(int elementNode, int x, int y, int w, int h) {

    this->iterateBounds(elementNode, x, y, w, h, &(this->cellRemove));
    this->elementRemove(elementNode);
}

template <class T>
void Grid<T>::update(int elementNode, int x1, int y1, int x2, int y2, int w, int h) {

    w++; // Add one to avoid entities close to border being missed because of conversion to int
    h++;

    assert((x2 >= 0 && y2 >= 0 && x2+w <= this->width && y2+h <= this->height) 
        && "Attempted out of bounds Grid update");
}

template <class T>
std::vector<T> Grid<T>::query(int x, int y, int w, int h) {

    this->lastQuery.clear();
    this->iterateBounds(-1, x, y, w, h, &(this->cellQuery));

    std::vector<T> results;
    results.reserve(this->lastQuery.size());
    
    for (auto element : this->lastQuery) {
        results.push_back(this->elements[this->elementNodes[element].element]);
    }

    return results;
}

template <class T>
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

template <class T>
void Grid<T>::elementRemove(int elementNode) {

    // Make the given elementNode the head of the freeElementNodes list
    this->elementNodes[elementNode].next = this->freeElementNodes;
    this->freeElementNodes = elementNode;
}

template <class T>
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

template <class T>
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

template <class T>
void Grid<T>::cellQuery(int cellNode, int unused) {

    int currentNode = this->cellNodes[cellNode].next;

    while (currentNode != -1) {
        this->lastQuery.insert(this->cellNodes[currentNode].element);
        currentNode = this->cellNodes[currentNode].next;
    }
}

template <class T>
void Grid<T>::iterateBounds(int node, int x, int y, int w, int h, void (Grid::*function)(int, int)) {

    w++; // Add one to avoid entities close to border being missed because of conversion to int
    h++;

    assert((x >= 0 && y >= 0 && x+w <= this->width && y+h <= this->height) 
        && "Attempted out of bounds operation");

    int xCellStart = x/this->cellSize;
    int yCellStart = y/this->cellSize;

    int xCellEnd = (w+this->cellSize-1)/this->cellSize + xCellStart;
    int yCellEnd = (h+this->cellSize-1)/this->cellSize + yCellStart;

    for (int xx{xCellStart}; xx < xCellEnd; xx++) {

        for (int yy{yCellStart}; yy < yCellEnd; yy++) {

            (this->*function)(yy*this->cellRowLength + xx, node);           
        }
    }
}

template <typename T>
class GridManager {
public:

    GridManager(entt::registry& registry);
    void init(int width, int height, int cellSize);
    std::vector<entt::entity> query(int x, int y, int w, int h);
    void clear();

private:

    void observeConstruct(entt::registry& registry, entt::entity entity);
    void observeDestroy(entt::registry& registry, entt::entity entity);
    void observeUpdate(entt::registry& registry, entt::entity entity);

    Grid<entt::entity> grid;
};

template <typename T>
GridManager<T>::GridManager(entt::registry& registry) {
    
    registry.on_construct<T>().connect<&GridManager<T>::observeConstruct>(this);
    registry.on_destroy<T>().connect<&GridManager<T>::observeDestroy>(this);
    registry.on_update<T>().connect<&GridManager<T>::observeUpdate>(this);
}

template <typename T>
void GridManager<T>::init(int width, int height, int cellSize) {
    this->grid.init(width, height, cellSize);
}

template <typename T>
std::vector<entt::entity> GridManager<T>::query(int x, int y, int w, int h) {
    return this->grid.query(x, y, w, h);
}

template <typename T>
void GridManager<T>::clear() {
    this->grid.clear();
}

template <typename T>
void GridManager<T>::observeConstruct(entt::registry& registry, entt::entity entity) {

}

template <typename T>
void GridManager<T>::observeDestroy(entt::registry& registry, entt::entity entity) {

}

template <typename T>
void GridManager<T>::observeUpdate(entt::registry& registry, entt::entity entity) {

}