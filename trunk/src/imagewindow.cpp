#include "imagewindow.h"
#include "ui_imagewindow.h"
#include <QMessageBox>
#include <QtCore/QtCore>
#include <QMouseEvent>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <qtextension.h>

const QImage *qi;

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
}

ImageLabel *label;

ImageWindow::ImageWindow(QWidget *parent, QString filesname) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ui->setupUi(this);
    qi=new QImage(filesname,0);

    for(int x=0;x<qi->height();x++){

        for(int y=0;y<qi->width();y++){

            //QRgb *pixel = (QRgb *)qi->scanLine(x);
            //pixel=(pixel+y);
            //*pixel = qRgba(255,0,0,255);

        }

    }

    //*pixel = qRgba(255,0,0,100);
    //int blue = qBlue(*pixel);
    /** Adding scroll:begin **/
    //ImageLabel *
            label=new ImageLabel(this);
    label->setAccessibleName("label");
    label->setObjectName("label");    
    //label->setMinimumHeight(480);
    //label->setMinimumWidth(640);
    label->setPixmap(QPixmap::fromImage(*qi,Qt::AutoColor));

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidget(label);
    scroll->setLayoutDirection(Qt::LayoutDirectionAuto);
    scroll->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    ui->horizontalLayout->addWidget(scroll);
    /** Adding scroll:end **/
    connect(label,SIGNAL(selected(QMouseEvent*)),this,SLOT(mouseOver(QMouseEvent*)));
}

void ImageWindow::save(QString fileName){
    qi->save(fileName,0,-1);
}

void ImageWindow::mouseOver(QMouseEvent* event){
  qDebug("%i,%i",event->pos().x(),event->pos().y());

}

ImageWindow::~ImageWindow()
{
    delete ui;
}

