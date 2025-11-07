#ifndef SPANNINGTREE_H
#define SPANNINGTREE_H

#include <cstdint>
#include <random>
#include <vector>

//   1
// 2 # 4
//   8
#define NORTH 0 // 1
#define WEST 1  // 2
#define EAST 2  // 4
#define SOUTH 3 // 8
#define LOCKED ((1 << NORTH) + (1 << WEST) + (1 << EAST) + (1 << SOUTH))

// A WallMatrix is a grid of uint8_t's that contain cell wall information
// std::vector is used to automatically fit storage requirements
using WallMatrix = std::vector<std::vector<uint8_t> >;
using Vertex = std::pair<int, int>;
using Vertices = std::vector<Vertex>;

class SpanningTree
{
public:
    SpanningTree();
    virtual ~SpanningTree() = default;

    virtual const std::string getName() const; // Runtime polymorphism: calls getNameStatic()
    static const std::string getNameStatic();  // Redefined by derived classes

    static uint8_t getAdj(uint8_t adj, int dir);
    static uint8_t setAdj(uint8_t adj, int dir, int val);

    virtual void calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit) = 0;

protected:
    std::random_device rdev;

    const static uint8_t dirVals[4]; // Values corresponding to NWES directions
    unsigned int rowEntry, rowExit;
    unsigned int rows, cols;

    void fillGraph(WallMatrix &matrix);
};

#endif // SPANNINGTREE_H
