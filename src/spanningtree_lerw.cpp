#include <algorithm>
#include <iostream>

#include "maze.h"
#include "spanningtree_lerw.h"

SpanningTreeLERW::SpanningTreeLERW() {}

const std::string SpanningTreeLERW::getNameStatic()
{
    return "Loop-Erased Random Walk";
}

void SpanningTreeLERW::calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit)
{
    std::default_random_engine eng(rdev()); // Seed the random number generator
    // int count = 0; // DEBUG
    // Vertices and directions for the random walk
    Vertices rwV;      // Vertices   of the random walk
    Directions rwDirs; // Directions of the random walk
    Vertex v, prevV, vtarget;
    Direction dir, prevDir;

    // Direction offsets for row and column: NORTH, WEST, EAST, SOUTH
    const int8_t dirRow[] = {-1, 0, 0, 1};
    const int8_t dirCol[] = {0, -1, 1, 0};

    // Create a direction vector
    std::vector<uint8_t> dirs;
    for (int i = 0; i < 4; i++) {
        dirs.push_back(dirVals[i]);
    }

    // Fill the matrix with walls
    fillGraph(matrix);

    this->rowEntry = rowEntry;
    this->rowExit = rowExit;
    rows = matrix.size();
    cols = matrix[0].size();

    // Build set of free vertices from the matrix
    Vertices freeV;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            freeV.push_back(Vertex(i, j));
        }
    }

    // Shuffle the free vertices to randomise the possible starting points
    std::shuffle(freeV.begin(), freeV.end(), eng);

    // Start with the first random vertex
    v = freeV.back();
    freeV.pop_back();

    // Choose a random neighbour and mark their edge (initiate the spanning tree)
    dir = (v.first > 0 ? NORTH : SOUTH); // NORTH if v is not in the first row
    vtarget = Vertex(v.first + dirRow[dir], v.second + dirCol[dir]);

    // Carve the wall between the current vertex and the selected neighbour
    matrix[v.first][v.second] = SpanningTree::setAdj(matrix[v.first][v.second], dir, 0);
    matrix[vtarget.first][vtarget.second]
        = SpanningTree::setAdj(matrix[vtarget.first][vtarget.second], INVDIR(dir), 0);

    // Remove the target from the free vertices
    freeV.erase(std::remove(freeV.begin(), freeV.end(), vtarget), freeV.end());

    // Perform random walks until all vertices are part of the spanning tree
    while (!freeV.empty()) {
        // Get a new start vertex for the random walk
        std::shuffle(freeV.begin(), freeV.end(), eng); // Shuffle the free vertices
        v = freeV.back();

        // Choose a random direction for the walk within the matrix bounds
        do {
            std::shuffle(dirs.begin(), dirs.end(), eng); // Shuffle the direction vector
            dir = dirs.back();
        } while ((dir == NORTH && v.first == 0) || (dir == SOUTH && v.first == rows - 1)
                 || (dir == WEST && v.second == 0) || (dir == EAST && v.second == cols - 1));

        // Perform a random walk until we hit the current spanning tree
        while (ISLOCKED(matrix[v.first][v.second])) {
            // Test the target of the random walk step
            vtarget = Vertex(v.first + dirRow[dir], v.second + dirCol[dir]);

            if (ISLOCKED(matrix[vtarget.first][vtarget.second]) == false) {
                // The target is part of the spanning tree: add the current walk to the tree
                matrix[v.first][v.second] = SpanningTree::setAdj(matrix[v.first][v.second], dir, 0);
                matrix[vtarget.first][vtarget.second]
                    = SpanningTree::setAdj(matrix[vtarget.first][vtarget.second], INVDIR(dir), 0);

                // Remove current vertex from the free vertices (now part of the spanning tree)
                freeV.erase(std::remove(freeV.begin(), freeV.end(), v), freeV.end());
                vtarget = v;

                // Add random walk to the spanning tree
                while (!rwV.empty()) {
                    prevV = rwV.back();
                    prevDir = rwDirs.back();
                    rwV.pop_back();
                    rwDirs.pop_back();
                    matrix[prevV.first][prevV.second]
                        = SpanningTree::setAdj(matrix[prevV.first][prevV.second], prevDir, 0);
                    matrix[vtarget.first][vtarget.second]
                        = SpanningTree::setAdj(matrix[vtarget.first][vtarget.second],
                                               INVDIR(prevDir),
                                               0);
                    freeV.erase(std::remove(freeV.begin(), freeV.end(), prevV), freeV.end());
                    vtarget = prevV;
                }
                break; // Break the random walk loop when a valid path is found
            } else if (std::find(rwV.begin(), rwV.end(), vtarget) != rwV.end()) {
                // Loop detected in the current random walk: erase the loop and backtrack
                while (rwV.back() != vtarget) {
                    prevV = rwV.back();
                    rwV.pop_back();
                    rwDirs.pop_back();
                    matrix[prevV.first][prevV.second] = LOCKED;
                    freeV.push_back(prevV);
                }
                v = rwV.back();
                dir = rwDirs.back();
                rwV.pop_back();
                rwDirs.pop_back();
                vtarget = Vertex(v.first + dirRow[dir], v.second + dirCol[dir]);
            }

            // Add the new vertex and direction to the random walk
            rwV.push_back(v);
            rwDirs.push_back(dir);
            v = vtarget;

            // Find a new direction within the matrix bounds
            do {
                std::shuffle(dirs.begin(), dirs.end(), eng); // Shuffle the direction vector
                dir = dirs.back();
            } while ((dir == NORTH && v.first == 0) || (dir == SOUTH && v.first == rows - 1)
                     || (dir == WEST && v.second == 0) || (dir == EAST && v.second == cols - 1));
        }
    }

    // Open the entry and exit
    matrix[rowEntry][0] = SpanningTree::setAdj(matrix[rowEntry][0], WEST, 0);
    matrix[rowExit][cols - 1] = SpanningTree::setAdj(matrix[rowExit][cols - 1], EAST, 0);
}
