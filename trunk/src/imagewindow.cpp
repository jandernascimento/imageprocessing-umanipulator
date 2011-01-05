#include "imagewindow.h"
#include "ui_imagewindow.h"

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
    const QImage *qi=new QImage(filesname/*QString("/home/jander/Desktop/P1030833.JPG")*/,0);

    //QPicture *p=new QPicture();

    ui->label->setPixmap(QPixmap::fromImage(*qi,Qt::AutoColor));

    //ui->verticalLayout->addWidget(qi);

    //const QList ql=new QList();
    //ql.append(qi);
    //centralWidget()->children().append(ql);
}

ImageWindow::~ImageWindow()
{
    delete ui;
}
