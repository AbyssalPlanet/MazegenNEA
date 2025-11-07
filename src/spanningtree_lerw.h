#ifndef SPANNINGTREE_LERW_H
#define SPANNINGTREE_LERW_H

#include "spanningtree.h"

typedef std::pair<int, int> Vertex;
typedef std::vector<Vertex> Vertices;
typedef int Direction;
typedef std::vector<Direction> Directions;

class SpanningTreeLERW : public SpanningTree
{
public:
    SpanningTreeLERW();
    static const std::string getNameStatic();
    void calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit);

protected:
    // DEBUG
    //void printRandomWalk(Directions &rwDirs, Vertices &rwV, Direction &dir, Vertex &v);
};

#endif // SPANNINGTREE_LERW_H
