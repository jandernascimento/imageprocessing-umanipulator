#include "imagewindow.h"
#include "ui_imagewindow.h"
#include <QMessageBox>
#include <QtCore/QtCore>
#include <QMouseEvent>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <custom/imagelabel.h>
#include <custom/imageabstration.h>

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{

    ui->setupUi(this);

}


ImageWindow::ImageWindow(QWidget *parent, QString filePath, QString fileName) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ImageLabel *label;
    ui->setupUi(this);
    ImageWindow::setWindowTitle(fileName);

    image=new ImageAbstraction(filePath,0);

    /** Adding scroll:begin **/
    label=new ImageLabel(this);
    label->setAccessibleName("label");
    label->setObjectName("label");    
    //label->setMinimumHeight(480);
    //label->setMinimumWidth(640);
    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidget(label);
    scroll->setLayoutDirection(Qt::LayoutDirectionAuto);
    scroll->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    ui->horizontalLayout->addWidget(scroll);
    /** Adding scroll:end **/
    connect(label,SIGNAL(selected(QMouseEvent*)),this,SLOT(mouseOver(QMouseEvent*)));
}

void ImageWindow::save(QString fileName){
    this->image->save(fileName,0,-1);
}

void ImageWindow::mouseOver(QMouseEvent* event){
  qDebug("%i,%i",event->pos().x(),event->pos().y());
  QRgb *pixel=image->getPixel(event->pos().x(),event->pos().y());
  QString message=QString("RGB(%1,%2,%3)").arg(qRed(*pixel)).arg(qGreen(*pixel)).arg(qBlue(*pixel));
  ui->statusbar->showMessage(message);
}

ImageWindow::~ImageWindow()
{
    delete ui;
}


