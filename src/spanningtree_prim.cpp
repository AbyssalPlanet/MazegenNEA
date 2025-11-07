#include "spanningtree_prim.h"
#include "maze.h"
#include <algorithm>

SpanningTreePrim::SpanningTreePrim() {}

const std::string SpanningTreePrim::getNameStatic()
{
    return "Prim's Algorithm";
}

void SpanningTreePrim::calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit)
{
    std::default_random_engine eng(rdev()); // Seed the generator

    fillGraph(matrix); // Fill the matrix with walls
    this->rowEntry = rowEntry;
    this->rowExit = rowExit;
    rows = matrix.size();
    cols = matrix[0].size();

    // Build list of unvisited vertices from the matrix
    Vertices freeV;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            freeV.push_back(Vertex(i, j));
        }
    }

    // Create a direction vector:
    std::vector<uint8_t> dirs;
    for (int i = 0; i < 4; i++) {
        dirs.push_back(dirVals[i]);
    };

    // Direction offsets for row and column movement: NORTH, WEST, EAST, SOUTH
    const int8_t dirRow[] = {-1, 0, 0, 1};
    const int8_t dirCol[] = {0, -1, 1, 0};

    // List of vertices in the maze spanning tree
    Vertices visitedV;

    // Shuffle the free vertices and get a random start point in the grid
    std::shuffle(freeV.begin(), freeV.end(), eng);
    Vertex v = freeV.back();
    freeV.pop_back();
    visitedV.push_back(v);

    // DEBUG
    //int count = 0;

    while (!freeV.empty()) {
        // Shuffle the visited vertices to choose a random one
        std::shuffle(visitedV.begin(), visitedV.end(), eng);

        bool foundVertex = false;

        // Randomly iterate over the visited vertices and add new neighbour vertices to the spanning tree
        for (Vertex v : visitedV) {
            // DEBUG
            // log size of freeV and visitedV
            // LOG(std::string("freeV.size() = ") + std::to_string(freeV.size())
            //     + std::string(", visitedV.size() = ") + std::to_string(visitedV.size()));

            // Shuffle the directions to get a random neighbour cell
            std::shuffle(dirs.begin(), dirs.end(), eng);

            // Try each direction to find an unvisited neighbour
            for (uint8_t dir : dirs) {
                //count++;

                // Calculate the row and column of the neighbouring cell
                int newRow = v.first + dirRow[dir];
                int newCol = v.second + dirCol[dir];

                // Check if new coords are within bounds
                if ((dir == NORTH && v.first > 0) || (dir == SOUTH && v.first < rows - 1)
                    || (dir == WEST && v.second > 0) || (dir == EAST && v.second < cols - 1)) {
                    // DEBUG
                    // LOG(std::string("matrix = ") + std::to_string(matrix[v.first][v.second])
                    //     + std::string(", ISLOCKED(matrix) = ")
                    //     + std::to_string(ISLOCKED(matrix[v.first][v.second]))
                    //     + std::string(", dir = ") + DIRNAME(dir));

                    // If the cell is unvisited: carve a passage
                    if (ISLOCKED(matrix[newRow][newCol])) {
                        // Carve wall in the current cell
                        matrix[v.first][v.second] = SpanningTree::setAdj(matrix[v.first][v.second],
                                                                         dir,
                                                                         0);

                        // Carve wall in the neighbouring cell
                        matrix[newRow][newCol] = SpanningTree::setAdj(matrix[newRow][newCol],
                                                                      INVDIR(dir),
                                                                      0);

                        // Remove neighbour vertex from the free vertices
                        freeV.erase(std::remove(freeV.begin(), freeV.end(), Vertex(newRow, newCol)),
                                    freeV.end());

                        // Add the new vertex to the visited list
                        visitedV.push_back(Vertex(newRow, newCol));

                        foundVertex = true;
                    }
                }
            }
            // If a neighbour vertex was found: break inner loop and continue
            if (foundVertex)
                break;
        }
    }
    // DEBUG: log count of iterations
    //LOG(std::string("count = ") + std::to_string(count));

    // Open the entry and exit walls
    matrix[rowEntry][0] = SpanningTree::setAdj(matrix[rowEntry][0], WEST, 0);
    matrix[rowExit][cols - 1] = SpanningTree::setAdj(matrix[rowExit][cols - 1], EAST, 0);
}
