#pragma once

#include <entt/entt.hpp>
#include <cstdint>
#include "FreeList.h"

// Adapted from: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

// Represents a node in the quadtree.
struct QuadNode {
    // Points to the first child if this node is a branch or the first
    // element if this node is a leaf.
    int32_t firstChild;

    // Stores the number of elements in the leaf or -1 if it this node is
    // not a leaf.
    int32_t count;
};

struct QuadNodeData {
    // Center x, center y, half width, half height
    int x, y, hw, hh;
    int index;
    int depth;
};

// Represents an element in the quadtree.
struct QuadElt {
    // Stores the ID for the element (can be used to
    // refer to external data).
    entt::entity entity;

    // Stores the rectangle for the element.
    int x, y, hw, hh;
};

// Represents an element node in the quadtree.
struct QuadEltNode {
    // Points to the next element in the leaf node. A value of -1 
    // indicates the end of the list.
    int next;

    // Stores the element index.
    int element;
};

class QuadTree {

public:

    QuadTree();
    // Converts empty children into leaf nodes and removes the children.
    // This process is defered until the end of each frame so that premature puning does not take place
    void cleanup();
    // Returns the index of the item, for removing or updating
    int insert(entt::entity, int x, int y, int w, int h);
    void remove(int index);
    void update(int index, int x, int y, int w, int h);
    std::vector<entt::entity> query(int x, int y, int w, int h);
    // Use function on all elements of the QuadTree in the most efficient manner
    template <typename F>
    void each(F func);

private:
    std::vector<QuadNodeData> getLeaves(int x, int y, int w, int h);
    void leafInsert();
    void leafRemove();


    // Stores all the elements in the quadtree.
    FreeList<QuadElt> elements;

    // Stores all the element nodes in the quadtree.
    FreeList<QuadEltNode> elementNodes;

    // Stores all the nodes in the quadtree. The first node in this
    // sequence is always the root.
    std::vector<QuadNode> nodes;

    // The quadtree half-width and half-size. It is always assumed that the top left is at 0,0
    int hw, hh;

    // Stores the first free node in the quadtree to be reclaimed as 4
    // contiguous nodes at once. A value of -1 indicates that the free
    // list is empty, at which point we simply insert 4 nodes to the
    // back of the nodes array.
    int freeNode;

    // Stores the maximum depth allowed for the quadtree.
    int maxDepth;
};