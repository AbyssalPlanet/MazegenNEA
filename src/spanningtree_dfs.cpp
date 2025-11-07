#include "spanningtree_dfs.h"
#include "maze.h"
#include <algorithm>
#include <iostream>

std::vector<std::pair<std::pair<int, int>, int>> directions = {
    {{-1, 0}, NORTH}, // Move NORTH (x, y, direction)
    {{1, 0}, SOUTH},  // Move SOUTH
    {{0, -1}, WEST},  // Move WEST
    {{0, 1}, EAST}    // Move EAST
};

SpanningTreeDFS::SpanningTreeDFS() {}

const std::string SpanningTreeDFS::getNameStatic()
{
    return "Depth-First Search";
}

void SpanningTreeDFS::calcSpanningTree(WallMatrix &matrix, int rowEntry, int rowExit)
{
    fillGraph(matrix);
    this->rowEntry = rowEntry;
    this->rowExit = rowExit;
    rows = matrix.size();
    cols = matrix[0].size();

    // Open the entry and exit:
    matrix[rowEntry][0] = SpanningTree::setAdj(matrix[rowEntry][0], WEST, 0);
    matrix[rowExit][cols - 1] = SpanningTree::setAdj(matrix[rowExit][cols - 1], EAST, 0);

    DepthFirstSearch(matrix, rowEntry, 0); // Start the search at the entry
}

void SpanningTreeDFS::DepthFirstSearch(WallMatrix &matrix, int row, int col)
{
    //static int count = 0;
    //count++; // DEBUG: Count the number of recursive calls
    std::default_random_engine eng(rdev()); // Seed the generator

    // Create a direction vector:
    std::vector<uint8_t> dirs;
    for (int i = 0; i < 4; i++) {
        dirs.push_back(dirVals[i]);
    }
    // Shuffle the direction vector
    std::shuffle(dirs.begin(), dirs.end(), eng);

    // Direction offsets for row and column
    const int8_t dirRow[] = {-1, 0, 0, 1}; // NORTH, WEST, EAST, SOUTH
    const int8_t dirCol[] = {0, -1, 1, 0}; // NORTH, WEST, EAST, SOUTH

    // Iterate through the shuffled direction vector
    for (uint8_t dir : dirs) {
        int8_t newRow = row + dirRow[dir];
        int8_t newCol = col + dirCol[dir];

        // Check bounds and if the cell is locked: perform recursive search
        if ((dir == NORTH && row > 0) || (dir == SOUTH && row < rows - 1)
            || (dir == WEST && col > 0) || (dir == EAST && col < cols - 1)) {
            // If the new vertex is the exit: carve into it immediately
            if (newRow == rowExit && newCol == cols - 1) {
                carveWall(matrix, row, col, dir, newRow, newCol);
                DepthFirstSearch(matrix, newRow, newCol);
                continue;
            }

            if (ISLOCKED(matrix[newRow][newCol])) {
                // Carve the wall between the current cell and the new cell
                carveWall(matrix, row, col, dir, newRow, newCol);

                // Recursively call DFS on the new cell
                DepthFirstSearch(matrix, newRow, newCol);
            }
        }
    }
}

void SpanningTreeDFS::carveWall(
    WallMatrix &matrix, int row, int col, uint8_t dir, int newRow, int newCol)
{
    // Carve the wall in both directions
    matrix[row][col] = setAdj(matrix[row][col], dir, 0);
    matrix[newRow][newCol] = setAdj(matrix[newRow][newCol], INVDIR(dir), 0);

    // Debug: print walls being carved
    // std::cout << "Carving path from (" << row << ", " << col << ") to ("
    //           << newRow << ", " << newCol << ")" << std::endl;
}
