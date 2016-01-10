#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //Initialize an application
    QApplication a(argc, argv);

    //Instantiate a MainWindow class and call the show() and initialize() function
    MainWindow w;
    w.show();
    w.initialize();

    return a.exec();
}
