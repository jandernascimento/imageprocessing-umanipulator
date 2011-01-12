#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "mainwindowactions.cpp"
#include <custom/imagelabel.h>
#include <custom/imageabstration.h>
#include <QScrollArea>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    createMenu();
    showMaximized();
    label=new ImageLabel(this);
    label->setAccessibleName("label");
    label->setObjectName("label");
    label->setMinimumHeight(480);
    label->setMinimumWidth(640);
    label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidget(label);
    scroll->setLayoutDirection(Qt::LayoutDirectionAuto);
    scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    ui->horizontalLayout->addWidget(scroll);

}

void MainWindow::createMenu(){

    //file
    filemenu=menuBar()->addMenu("&File");
    setupFileMenu(filemenu);

    //view
    viewmenu=menuBar()->addMenu("&View");
    setupViewMenu(viewmenu);

    //image
    imagemenu=menuBar()->addMenu("&Image");
    setupImageMenu(imagemenu);

    //image/filter
    QMenu *imagesubfilter=imagemenu->addMenu("&Filter");
    setupImageFilterSubMenu(imagesubfilter);

    //image/advanced
    imagemenu->addSeparator();
    QMenu *imagesubadvanced=imagemenu->addMenu("&Advanced");
    setupImageAdvancedSubMenu(imagesubadvanced);

    //color
    colormenu=menuBar()->addMenu("&Color");
    setupColorMenu(colormenu);


}


void MainWindow::setupFileMenu(QMenu *menu){
    QAction *openaction=new QAction(("&Open..."), this);
    QAction *saveaction=new QAction(("&Save"), this);
    QAction *saveasaction=new QAction(("Save &as..."), this);
    QAction *quitaction=new QAction(("&Quit"), this);

    configureOpen(openaction);
    configureSave(saveaction);
    configureSaveAs(saveasaction);
    configureQuit(quitaction);

    menu->addAction(openaction);
    menu->addAction(saveaction);
    menu->addAction(saveasaction);
    menu->addSeparator();
    menu->addAction(quitaction);
}

void MainWindow::setupViewMenu(QMenu *menu){

    QAction *colorbaraction=new QAction(("&Color information bar"), this);
    QAction *histogramaction=new QAction(("&Histogram"), this);

    configureHistogram(histogramaction);

    menu->addAction(colorbaraction);
    menu->addAction(histogramaction);

}

void MainWindow::setupImageMenu(QMenu *menu){
    QAction *crop=new QAction(("&Crop"), this);
    QAction *blur=new QAction(("&Blur"), this);
    QAction *fusion=new QAction(("&Fusion"), this);
    QAction *resize=new QAction(("&Resize"), this);

    crop->setCheckable(true);

    configureFusion(fusion);
    connect(blur, SIGNAL(triggered()),this,SLOT(applyBlur()));

    connect(crop, SIGNAL(triggered()),this,SLOT(applyCrop()));
    menu->addAction(crop);
    menu->addAction(blur);
    menu->addAction(fusion);
    menu->addAction(resize);
}

void MainWindow::setupImageFilterSubMenu(QMenu *menu){
    QAction *mean=new QAction(("Mean"), this);
    QAction *gradient=new QAction(("Gradiant"), this);
    QAction *laplacian=new QAction(("Laplacian"), this);
    QAction *custom=new QAction(("Custom"), this);
    connect(custom, SIGNAL(triggered()),this,SLOT(applyBlurCustomDialog()));
    connect(mean, SIGNAL(triggered()),this,SLOT(applyMeanFilter()));
    menu->addAction(mean);
    menu->addAction(gradient);
    menu->addAction(laplacian);
    menu->addSeparator();
    menu->addAction(custom);

}

void MainWindow::setupImageAdvancedSubMenu(QMenu *menu){
    QAction *resizing=new QAction(("Intelligent resizing"), this);
    QAction *scissor=new QAction(("Intelligent scissor"), this);

    menu->addAction(resizing);
    menu->addAction(scissor);
}

void MainWindow::setupColorMenu(QMenu *menu){
    QAction *contrast=new QAction(("&Contrast and equalization"), this);
    QAction *convertGrey=new QAction(("Convert to &grey scale"), this);

    configureContrast(contrast);
    configureGrey(convertGrey);

    menu->addAction(contrast);
    menu->addAction(convertGrey);
}

void MainWindow::configureQuit(QAction *act){    
    connect(act, SIGNAL(triggered()),this,SLOT(quit()));
}

void MainWindow::configureOpen(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(open()));
}

void MainWindow::configureSave(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(save()));
}

void MainWindow::configureContrast(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(dialogContrast()));
}

void MainWindow::configureGrey(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(applyGrey()));
}
void MainWindow::configureSaveAs(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(saveas()));
}

void MainWindow::mouseOver(QMouseEvent* event){

   int x=event->pos().x();
   int y=event->pos().y();

  QRgb *pixel=image->getPixel(event->pos().x(),event->pos().y());
  QString message=QString("RGB(%1,%2,%3) CMYK(%4, %5, %6, %7)")
                  .arg(qRed(*pixel))
                  .arg(qGreen(*pixel))
                  .arg(qBlue(*pixel))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::cyan))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::magenta))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::yellow))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::black));

  ui->statusBar->showMessage(message);
}

void MainWindow::save(QString fileName){
    this->image->save(fileName,0,-1);
}

void MainWindow::configureHistogram(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(histogram()));
}

void MainWindow::configureFusion(QAction *act){

    connect(act, SIGNAL(triggered()),this, SLOT(dialogFusion()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


