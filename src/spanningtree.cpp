#include "spanningtree.h"

// Define the static member variable outside the class
const uint8_t SpanningTree::dirVals[4] = {NORTH, WEST, EAST, SOUTH};

SpanningTree::SpanningTree() {}

const std::string SpanningTree::getName() const
{
    return getNameStatic();
}

const std::string SpanningTree::getNameStatic()
{
    // Redefined by derived classes
    return "Base SpanningTree";
}

// Fill the labyrinth with walls everywhere
void SpanningTree::fillGraph(WallMatrix &matrix)
{
    int rows = matrix.size();
    int cols = matrix[0].size();
    uint8_t val = LOCKED;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = val;
        }
    }
}

// Get the value of the adjacent cell in a given direction
uint8_t SpanningTree::getAdj(uint8_t adj, int dir)
{
    return (adj >> dir) & 1;
}

// Set the value of the adjacent cell in a given direction
uint8_t SpanningTree::setAdj(uint8_t adj, int dir, int val)
{
    // clear the position and replace with a new value
    return ((adj & ~(1 << dir)) | ((val != 0) << dir));
}
