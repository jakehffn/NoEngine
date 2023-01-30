#pragma once

#include <vector>
#include <set>

struct Node {
    // Index of element in element list
    int element;
    // Either the index of the next element in the cell or the next element in the free list
    // -1 if the end of either list
    int next; 
};

template <class T>
class Grid {

public:
    Grid(int width, int height, int cellSize);
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

    void iterateBounds(int node, int x, int y, int w, int h, void (Grid::*)(int, int) function);

    std::vector<T> elements;
    std::vector<Node> elementNodes;
    std::vector<Node> cellNodes; // The first cells in this list will never change and will be accessed directly, acting as the 2D list of cells

    std::set<int> lastQuery;

    int freeElementNodes=-1; // singly linked-list of the free nodes
    int freeCellNodes=-1; 

    const int width;
    const int height;
    const int cellSize;
};