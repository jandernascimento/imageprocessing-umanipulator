#include <QtGui/QApplication>
#include "mainwindow.h"
#include "imagewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ImageWindow w2;
    w2.show();

    return a.exec();
}
