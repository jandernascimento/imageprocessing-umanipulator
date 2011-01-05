#include "imagewindow.h"
#include "ui_imagewindow.h"
#include <QMessageBox>
#include <QtCore/QtCore>
#include <QMouseEvent>

const QImage *qi;

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
    qi=new QImage(filesname,0);

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

void ImageWindow::save(QString fileName){
    qi->save(fileName,0,-1);
}

ImageWindow::~ImageWindow()
{
    delete ui;
}
