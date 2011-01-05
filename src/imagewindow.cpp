#include "imagewindow.h"
#include "ui_imagewindow.h"
#include <QMessageBox>
#include <QtCore/QtCore>
#include <QMouseEvent>
#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>

const QImage *qi;

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow)
{
    ui->setupUi(this);

    QLabel *l1=new QLabel();
    l1->setAccessibleName("label");
    l1->setObjectName("label");
    l1->setText("vvvaaaaai");
    l1->setMinimumHeight(300);
    l1->setMinimumWidth(400);

    ui->horizontalLayout->addWidget(l1);

    l1->show();
}

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
    QLabel *label=new QLabel(this);
    label->setAccessibleName("label");
    label->setObjectName("label");    
    label->setMinimumHeight(480);
    label->setMinimumWidth(640);
    label->setPixmap(QPixmap::fromImage(*qi,Qt::AutoColor));

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidget(label);
    scroll->setLayoutDirection(Qt::LayoutDirectionAuto);
    scroll->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    ui->horizontalLayout->addWidget(scroll);
    /** Adding scroll:end **/

}

void ImageWindow::save(QString fileName){
    qi->save(fileName,0,-1);
}

ImageWindow::~ImageWindow()
{
    delete ui;
}

