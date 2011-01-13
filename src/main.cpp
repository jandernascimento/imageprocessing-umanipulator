#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDir>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //QDir::addResourceSearchPath("resources")

    //Q_INIT_RESOURCE();

    MainWindow w;
    w.show();

    return a.exec();
}
