#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeySequence>
#include <QPainter>
#include <QPen>
#include <QResizeEvent>
#include <QShortcut>
#include <QTimer>

#include "mazerenderer.h"
#include <iostream>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , maze(nullptr)
    , renderer(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("MazegenNEA");

    connect(ui->exitPushButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->generationPushButton, &QPushButton::pressed, this, &MainWindow::onGenerate);
    connect(ui->splitter_1, &QSplitter::splitterMoved, this, &MainWindow::fitMazeToGraphicsViewSlot);
    connect(ui->splitter_2, &QSplitter::splitterMoved, this, &MainWindow::fitMazeToGraphicsViewSlot);
    connect(ui->solvingCheckBox, &QCheckBox::toggled, this, &MainWindow::onToggleSolution);
    connect(ui->wallColorPushButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);

    ui->splitter_1->insertWidget(0, ui->mazeGraphicsView);

    renderer = new MazeRenderer(maze);

    QTimer::singleShot(0, this, &MainWindow::fitMazeToGraphicsView);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete maze;
    delete renderer;
}

void MainWindow::onGenerate()
{
    std::cout << "Clicked, onGenerate() called." << std::endl;

    // Delete the existing maze and renderer to prevent memory leaks

    delete maze;
    maze = nullptr;

    delete renderer;
    renderer = nullptr;

    // Get row and column values from the ui
    unsigned int rows = ui->rowsSpinBox->value();
    unsigned int cols = ui->colsSpinBox->value();
    std::cout << "Rows: " << rows << "\nCols: " << cols << std::endl;

    // Get the selected generation algorithm from the ui
    std::string genAlg = ui->generationComboBox->currentText().toStdString();
    std::cout << "genAlg: " << genAlg << std::endl;

    // Create new maze object depending on the selected algorithm
    if (genAlg == SpanningTreeDFS::getNameStatic()) {
        maze = new Maze(rows, cols, new SpanningTreeDFS());
    } else if (genAlg == SpanningTreePrim::getNameStatic()) {
        maze = new Maze(rows, cols, new SpanningTreePrim());
    } else if (genAlg == SpanningTreeLERW::getNameStatic()) {
        maze = new Maze(rows, cols, new SpanningTreeLERW());
    } else {
        std::cerr << "onGenerate(): invalid generation type" << std::endl;
        return;
    }

    maze->generate();
    maze->solve();

    // Create a new renderer for the new maze
    renderer = new MazeRenderer(maze);
    renderer->setWallColor(wallColor);

    if (!renderer) {
        std::cerr << "Error: Renderer is null!" << std::endl;
        return;
    }

    // Render the maze with or without the solution directly into the graphics view
    renderer->renderMaze(ui->mazeGraphicsView, ui->solvingCheckBox->isChecked());

    // Ensure the view fits the maze
    fitMazeToGraphicsView();
}

void MainWindow::onToggleSolution(bool checked)
{
    if (renderer) {
        // Toggle the solution path (display with or without solution)
        renderer->renderMaze(ui->mazeGraphicsView, checked);

        // Ensure the view fits the maze
        fitMazeToGraphicsView();
    }
}

// Slot for the color picker
void MainWindow::openColorPicker()
{
    QColor selectedColor = QColorDialog::getColor(wallColor, this, "Choose Wall Color");

    if (selectedColor.isValid()) {
        // Update the wall colour
        wallColor = selectedColor;
        //qDebug() << "Wall colour changed to:" << wallColor.name();

        // Update the maze renderer with the new colour
        if (renderer) {
            renderer->setWallColor(wallColor);
            renderer->renderMaze(ui->mazeGraphicsView, ui->solvingCheckBox->isChecked());
            fitMazeToGraphicsView();
        }
    }
}

// Print the maze to std::cout for testing purposes
void MainWindow::displayMazeText()
{
    std::cout << "Clicked, displayMazeText() called." << std::endl;
    bool isSolutionPrinted = true;
    Maze maze(20, 20, new SpanningTreePrim());
    maze.generate(); // Generate the maze (spanning tree)
    maze.solve();    // Solve the maze
    maze.PrintMazeText(isSolutionPrinted);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (ui->mazeGraphicsView->scene()) {
        ui->mazeGraphicsView->fitInView(ui->mazeGraphicsView->scene()->itemsBoundingRect(),
                                        Qt::KeepAspectRatio);
    }
}

void MainWindow::fitMazeToGraphicsView()
{
    if (ui->mazeGraphicsView->scene()) {
        ui->mazeGraphicsView->fitInView(ui->mazeGraphicsView->scene()->itemsBoundingRect(),
                                        Qt::KeepAspectRatio);
    }
}

// Wrapper slot for fitMazeToGraphicsView() with unused parameters from splitterMoved()
// Required since Qt slots and signals must have matching parameters
void MainWindow::fitMazeToGraphicsViewSlot(int /*pos*/, int /*index*/)
{
    fitMazeToGraphicsView();
}
