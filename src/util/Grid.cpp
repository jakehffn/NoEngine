#include "Grid.h"

template <class T>
Grid<T>::Grid(int width, int height, int cellSize) : 
    width{ width }, height{ height }, cellSize{ cellSize } {

        int numCells = (width+cellSize-1)*(height+cellSize-1)/(cellSize*cellSize);

        this->cellNodes = std::vector(numCells);

        for (int xx{0}; xx < this->cells.capacity(); xx++) {
            cellNodes[xx] = (Node){-1, -1};
        }
}

template <class T>
int Grid<T>::insert(T element, int x, int y, int w, int h) {

    int newElementNode = this->elementInsert(element);
    this->iterateBounds(newElementNode, x, y, w, h, &(this->cellInsert);

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

    assert((x2 > 0 && y2 > 0 && x2+w < this->width && y2+h < this->height) 
        && "Attempted out of bounds Grid update");
}

template <class T>
std::vector<T> Grid<T>::query(int x, int y, int w, int h) {

    this->lastQuery.clear();
    this->iterateBounds(-1, x, y, w, h, &(this->cellQuery));

    std::vector<T> results = std::vector(this->lastQuery.size());
    for (auto element : this->lastQuery) {
        results.push_back(this->elements[this->elementNodes[element]]);
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
        this->cellNodes[this->freeCellNodes].element = this->cellNodes[this->freeCellNodes].next

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

    assert((x > 0 && y > 0 && x+w < this->width && y+h < this->height) 
        && "Attempted out of bounds operation");

    int xCellStart = x/this->cellSize;
    int yCellStart = y/this->cellSize;

    int wCells = (w+this->cellSize-1)/this->cellSize;
    int hCells = (h+this->cellSize-1)/this->cellSize;

    for (int xx{xCellStart}; xx < xCellStart + wCells; xx++) {

        for (int yy{yCellStart}; yy < yCellStart + hCells; yy++) {

            (this->*function)(yy*this->width + xx, node);           
        }
    }
}