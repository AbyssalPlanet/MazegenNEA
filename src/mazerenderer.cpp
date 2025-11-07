#include "mazerenderer.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QPainter>
#include <QSvgGenerator>
#include "maze.h"
#include <iostream>

static const int CELL_SIZE = 10;     // Size of each maze cell in pixels
static const int WALL_THICKNESS = 1; // Thickness of maze walls

MazeRenderer::MazeRenderer(Maze *maze)
    : maze(maze)
    , wallColor(Qt::red)
{}

void MazeRenderer::renderMaze(QGraphicsView *view, bool drawSolution)
{
    if (!view)
        return;
    QGraphicsScene *scene = view->scene();
    if (!scene) {
        scene = new QGraphicsScene();
        view->setScene(scene);
    }

    scene->clear();

    int rows = maze->getRows();
    int cols = maze->getCols();

    scene->setSceneRect(0, 0, cols * CELL_SIZE, rows * CELL_SIZE);

    // Draw maze walls
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = col * CELL_SIZE;
            int y = row * CELL_SIZE;

            // Top wall
            if (maze->getWall(row, col, NORTH)) {
                scene->addLine(x, y, x + CELL_SIZE, y, QPen(wallColor, WALL_THICKNESS));
            }
            // Left wall
            if (maze->getWall(row, col, WEST)) {
                scene->addLine(x, y, x, y + CELL_SIZE, QPen(wallColor, WALL_THICKNESS));
            }
            // Bottom wall (only if last row or unique)
            if (row == rows - 1 || maze->getWall(row, col, SOUTH)) {
                scene->addLine(x,
                               y + CELL_SIZE,
                               x + CELL_SIZE,
                               y + CELL_SIZE,
                               QPen(wallColor, WALL_THICKNESS));
            }
            // Right wall (skip if this is the top-right corner)
            if ((col == cols - 1 || maze->getWall(row, col, EAST))
                && !(row == 0 && col == cols - 1)) {
                scene->addLine(x + CELL_SIZE,
                               y,
                               x + CELL_SIZE,
                               y + CELL_SIZE,
                               QPen(wallColor, WALL_THICKNESS));
            }

            // Draw solution path
            if (drawSolution && maze->isSolutionCell(row, col)) {
                QGraphicsRectItem *solutionItem = new QGraphicsRectItem(x + 2,
                                                                        y + 2,
                                                                        CELL_SIZE - 4,
                                                                        CELL_SIZE - 4);
                solutionItem->setBrush(QBrush(Qt::blue));
                solutionItem->setPen(Qt::NoPen);
                scene->addItem(solutionItem);
            }
        }
    }
}

void MazeRenderer::setWallColor(const QColor &color)
{
    wallColor = color;
}

QColor MazeRenderer::getWallColor() const
{
    return wallColor;
}
