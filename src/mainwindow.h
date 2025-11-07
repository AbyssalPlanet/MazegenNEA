#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maze.h"
#include "mazerenderer.h"
#include "spanningtree_dfs.h"
#include "spanningtree_lerw.h"
#include "spanningtree_prim.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onGenerate();
    void displayMazeText();
    void fitMazeToGraphicsViewSlot(int /*pos*/, int /*index*/);
    void onToggleSolution(bool checked);
    void openColorPicker();

private:
    Ui::MainWindow *ui;

    // Declare pointers for maze and renderer
    Maze *maze;
    MazeRenderer *renderer;

    QColor wallColor = Qt::red; // Default wall colour

    void fitMazeToGraphicsView();
};

#endif // MAINWINDOW_H
