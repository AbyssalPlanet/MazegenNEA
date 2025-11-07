#ifndef SPANNINGTREE_PRIM_H
#define SPANNINGTREE_PRIM_H

#include "spanningtree.h"

class SpanningTreePrim : public SpanningTree
{
public:
    SpanningTreePrim();

    static const std::string getNameStatic();
    void calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit);
};

#endif // SPANNINGTREE_PRIM_H
