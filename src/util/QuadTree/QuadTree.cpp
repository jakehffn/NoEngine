#include "QuadTree.h"

std::vector<QuadNodeData> QuadTree::getLeaves(int x, int y, int w, int h) {
    
    std::vector<QuadNodeData> leaves, toProcess;
    QuadNode root = this->nodes[0];

    toProcess.push_back((QuadNodeData){this->hw, this->hh,this->hw, this->hh, 0, 0});
    while (toProcess.size() > 0) {

        const QuadNodeData nd = toProcess.back();
        toProcess.pop_back();

        // If this node is a leaf, insert it to the list.
        if (this->nodes[nd.index].count != -1) {
            leaves.push_back(nd);
        } else {
            // Otherwise push the children that intersect the rectangle.
            const int mx = nd.x, my = nd.y;
            const int newhw = nd.hw >> 1, newhh = nd.hh >> 1;
            const int fc = this->nodes[nd.index].firstChild;
            const int l = mx-newhw, t = my-newhh, r = mx+newhw, b = my+newhh;

            if (y <= my) {
                if (x <= mx)
                    toProcess.push_back((QuadNodeData){l,t, newhw, newhh, fc+0, nd.depth+1});
                if (x+w > mx)
                    toProcess.push_back((QuadNodeData){r,t, newhw, newhh, fc+1, nd.depth+1});
            } if (y+h > my) {
                if (x <= mx)
                    toProcess.push_back((QuadNodeData){l,b, newhw, newhh, fc+2, nd.depth+1});
                if (x+w > mx)
                    toProcess.push_back((QuadNodeData){r,b, newhw, newhh, fc+3, nd.depth+1});
            }
        }
    }
    return leaves;
}

void QuadTree::cleanup() {

    // Only process the root if it's not a leaf.
    SmallList<int> toProcess;
    if (this->nodes[0].count == -1)
        toProcess.push_back(0);

    while (toProcess.size() > 0) {
        const int nodeIndex = toProcess.pop_back();
        QuadNode& node = nodes[nodeIndex];

        // Loop through the children.
        int numEmptyLeaves = 0;
        for (int j=0; j < 4; ++j) {
            const int childIndex = node.firstChild + j;
            const QuadNode& child = this->nodes[childIndex];

            // Increment empty leaf count if the child is an empty 
            // leaf. Otherwise if the child is a branch, add it to
            // the stack to be processed in the next iteration.
            if (child.count == 0)
                ++numEmptyLeaves;
            else if (child.count == -1)
                toProcess.push_back(childIndex);
        }

        // If all the children were empty leaves, remove them and 
        // make this node the new empty leaf.
        if (numEmptyLeaves == 4) {
            // Push all 4 children to the free list.
            this->nodes[node.firstChild].firstChild = this->freeNode;
            this->freeNode = node.firstChild;

            // Make this node the new empty leaf.
            node.firstChild = -1;
            node.count = 0;
        }
    }
}

