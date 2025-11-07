#ifndef MAZE_H
#define MAZE_H

#include "spanningtree.h"
#include <cstdint>

#define SOLUTION 4   // Cells marked as a SOLUTION cell are checked when rendering the maze
#define DIRMASK 0x0F // Mask for the direction bits
#define ISLOCKED(val) (((val) & DIRMASK) == LOCKED)
#define INVDIR(dir) (3 - (dir)) // Inverse direction
#define DIRNAME(dir) \
    ((dir == NORTH) ? "NORTH" : (dir == WEST) ? "WEST" : (dir == EAST) ? "EAST" : "SOUTH")

// Forward declaration of SpanningTree
// Becomes a derived class via runtime polymorphism
class SpanningTree;

class Maze
{
public:
    Maze(unsigned int rows, unsigned int cols);
    Maze(unsigned int rows, unsigned int cols, SpanningTree *sptAlgorithm);
    ~Maze();

    void generate();
    void solve();
    void PrintMazeText();
    void PrintMazeText(bool isSolutionPrinted);

    bool isSolutionCell(int row, int col) const;

    int getCols() const { return cols; }
    int getRows() const { return rows; }
    bool getWall(int row, int col, int dir) const;

protected:
    const static uint8_t dirVals[4];

    unsigned int rows;
    unsigned int cols;
    unsigned int rowEntry;
    unsigned int rowExit;

    WallMatrix wallMatrix;
    SpanningTree *sptAlgorithm;

    void clearSolution(); // clear the solution path
    bool SolveDFS(int row, int col);
};

#endif // MAZE_H
