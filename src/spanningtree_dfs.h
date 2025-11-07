#ifndef SPANNINGTREE_DEPTHFIRST_H
#define SPANNINGTREE_DEPTHFIRST_H

#include "spanningtree.h"

class SpanningTreeDFS : public SpanningTree
{
public:
    SpanningTreeDFS();
    static const std::string getNameStatic();

    void calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit);

protected:
    void DepthFirstSearch(WallMatrix &matrix, int row, int col);

private:
    void carveWall(WallMatrix &matrix, int row, int col, uint8_t dir, int newRow, int newCol);
};

#endif // SPANNINGTREE_DEPTHFIRST_H
