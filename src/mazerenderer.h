#ifndef MAZERENDERER_H
#define MAZERENDERER_H

#include <QColor>
#include <QGraphicsView>
#include <QString>
#include "maze.h"

class MazeRenderer
{
public:
    MazeRenderer(Maze *maze);

    // Render maze with or without solution
    void renderMaze(QGraphicsView *view, bool drawSolution);

    // Setter and Getter for wallColor
    void setWallColor(const QColor &color); // setter for wallColor
    QColor getWallColor() const;            // getter for wallColor

private:
    Maze *maze;        // Pointer to the Maze object
    QColor wallColor;  // Colour for the maze walls
};

#endif // MAZERENDERER_H
