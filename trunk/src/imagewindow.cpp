#include "imagewindow.h"
#include "ui_imagewindow.h"
#include <QtCore/QtCore>

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
}

ImageWindow::ImageWindow(QWidget *parent, QString filesname) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
    const QImage *qi=new QImage(filesname,0);

    for(int x=0;x<qi->height();x++){

        for(int y=0;y<qi->width();y++){

            QRgb *pixel = (QRgb *)qi->scanLine(x);
            pixel=(pixel+y);
            //*pixel = qRgba(255,0,0,255);

        }

    }

    //*pixel = qRgba(255,0,0,100);
    //int blue = qBlue(*pixel);

    ui->label->setPixmap(QPixmap::fromImage(*qi,Qt::AutoColor));

}

ImageWindow::~ImageWindow()
{
    delete ui;
}
