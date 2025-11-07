#include "maze.h"
//#include "debug.h"
#include "spanningtree_dfs.h"
#include <iostream>
#include <random>

// Foreground: 30=black, 31=red, 32=green, 33=yellow, 34=blue, 35=purple, 36=cyan, 37=white
// Background: 40=black, 41=red, 42=green, 43=yellow, 44=blue, 45=purple, 46=cyan, 47=white
#define BCOLOR1(txt) "\033[41m" << (txt) << "\033[0m" // border background color
#define BCOLOR2(txt) "\033[42m" << (txt) << "\033[0m" // solution background color

#define SET_SOLUTION_COLOR() std::cout << "\033[33m" << "\033[40m" // solution color
#define RESET_COLOR() std::cout << "\033[0m"                       // standard colors

// Fill with leading space to 3 digits
#define NFILL(num) (num < 10 ? "  " : (num < 100 ? " " : "")) << num

const uint8_t Maze::dirVals[4] = {NORTH, WEST, EAST, SOUTH};

// Use SpanningTreeDFS as the default sptAlgorithm
Maze::Maze(unsigned int rows, unsigned int cols)
    : Maze(rows, cols, new SpanningTreeDFS())
{}

Maze::Maze(unsigned int rows, unsigned int cols, SpanningTree *sptAlgorithm)
{
    this->rows = rows;
    this->cols = cols;
    this->rowExit = 0;
    this->rowEntry = rows - 1;

    // Initialise the matrix with all values set to 0
    wallMatrix = WallMatrix(rows, std::vector<uint8_t>(cols, 0));

    this->sptAlgorithm = sptAlgorithm;
}

Maze::~Maze()
{
    delete sptAlgorithm;
}

void Maze::generate()
{
    sptAlgorithm->calcSpanningTree(wallMatrix, rowEntry, rowExit);
}

void Maze::solve()
{
    // Clear the solution path
    clearSolution();

    // Mark entry as solution path
    wallMatrix[rowEntry][0] = SpanningTree::setAdj(wallMatrix[rowEntry][0], SOLUTION, 1);

    // Solve the maze
    SolveDFS(rowEntry, 0);
}

bool Maze::SolveDFS(int row, int col)
{
    //static int count = 0;
    // count++; // DEBUG: Count the number of recursive calls
    int dir;

    // Direction offsets for row and column
    const int8_t dirRow[] = {-1, 0, 0, 1}; // NORTH, WEST, EAST, SOUTH
    const int8_t dirCol[] = {0, -1, 1, 0}; // NORTH, WEST, EAST, SOUTH

    // Check if the exit is found
    if (row == rowExit && col == cols - 1) {
        return true; // exit found
    }

    // Iterate the shuffled direction vector
    for (int i = 0; i < 4; i++) {
        dir = dirVals[i];
        int8_t newRow = row + dirRow[dir];
        int8_t newCol = col + dirCol[dir];

        // Check bounds and if the cell is locked: perform recursive search
        if ((dir == NORTH && row > 0) || (dir == SOUTH && row < rows - 1)
            || (dir == WEST && col > 0) || (dir == EAST && col < cols - 1)) {
            if (SpanningTree::getAdj(wallMatrix[row][col], dir) == 0
                && SpanningTree::getAdj(wallMatrix[newRow][newCol], SOLUTION) == 0) {
                wallMatrix[newRow][newCol] = SpanningTree::setAdj(wallMatrix[newRow][newCol],
                                                                  SOLUTION,
                                                                  1); // mark as solution path
                if (SolveDFS(newRow, newCol)) {
                    return true;
                } else {
                    wallMatrix[newRow][newCol] = SpanningTree::setAdj(wallMatrix[newRow][newCol],
                                                                      SOLUTION,
                                                                      0); // clear the solution path
                }
            }
        }
    }
    return false;
}

void Maze::clearSolution()
{
    // clear the solution path
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            wallMatrix[i][j] = SpanningTree::setAdj(wallMatrix[i][j], SOLUTION, 0);
        }
    }
}

// Check if a cell is part of the solution path
bool Maze::isSolutionCell(int row, int col) const
{
    return SpanningTree::getAdj(wallMatrix[row][col], SOLUTION) != 0;
}

// Print the maze to the console
void Maze::PrintMazeText()
{
    PrintMazeText(false);
}

void Maze::PrintMazeText(bool isSolutionPrinted)
{
    // Print number position labels
    bool bNumberedTiles = false;
    bool printAxisLabels = true;

    std::cout << std::endl
              << "Content of Maze (" << sptAlgorithm->getName() << "): (" << rows << ", " << cols
              << ")\n"
              << std::endl;

    //---------------------------------------------------------------
    // Print coloumn labels
    if (printAxisLabels) {
        std::cout << "   ";
        for (int j = 0; j < cols; j++) {
            std::cout << (bNumberedTiles ? " " : "") << NFILL(j) << " ";
        }
        std::cout << " ";
        std::cout << std::endl;
    }
    //---------------------------------------------------------------
    // Print a top line
    if (printAxisLabels)
        std::cout << "   ";
    for (int j = 0; j < cols; j++) {
        std::cout << BCOLOR1(bNumberedTiles ? "     " : "    ");
    }
    std::cout << BCOLOR1(" ");
    std::cout << std::endl;
    for (int i = 0; i < rows; i++) {
        if (printAxisLabels)
            std::cout << NFILL(i);
        for (int j = 0; j < cols; j++) {
            if (SpanningTree::getAdj(wallMatrix[i][j], WEST)) {
                std::cout << BCOLOR1(" ") << "";
            } else {
                std::cout << " ";
            }

            // Set the solution background color
            if (isSolutionPrinted && SpanningTree::getAdj(wallMatrix[i][j], SOLUTION))
                SET_SOLUTION_COLOR();
            if (bNumberedTiles) {
                std::cout << NFILL(static_cast<int>(wallMatrix[i][j] & DIRMASK)) << " ";
            } else {
                std::cout << " . ";
            }
            RESET_COLOR();
        }
        if (SpanningTree::getAdj(wallMatrix[i][cols - 1], EAST)) {
            std::cout << BCOLOR1(" ");
        } else {
            std::cout << " ";
        }
        std::cout << std::endl;

        //---------------------------------------------------------------
        // Print the SOUTH border line for current row
        if (printAxisLabels)
            std::cout << "   ";
        for (int j = 0; j < cols; j++) {
            if (SpanningTree::getAdj(wallMatrix[i][j], SOUTH)) {
                std::cout << BCOLOR1(bNumberedTiles ? ("     ") : "    ");
            } else {
                std::cout << BCOLOR1(" ") << (bNumberedTiles ? "    " : "   ");
            }
        }
        std::cout << BCOLOR1(" ");
        std::cout << std::endl;
    }
}

bool Maze::getWall(int row, int col, int dir) const
{
    return SpanningTree::getAdj(wallMatrix[row][col], dir);
}
