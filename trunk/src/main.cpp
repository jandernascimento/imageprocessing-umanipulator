#include <QtGui/QApplication>
#include "mainwindow.h"
#include "dialogcontrast.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //DialogContrast dialog;// = new DialogContrast();

    //dialog.show();
    //dialog.setWindow(&w);

    return a.exec();
}
