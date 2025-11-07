#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QWidget>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;
    win.show();

    //std::cout << "TEST !!!" << std::endl;

    return app.exec();
}
