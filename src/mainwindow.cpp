#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

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
    QAction *openaction=new QAction(("&Open.."), this);
    QAction *saveaction=new QAction(("&Save"), this);
    QAction *saveasaction=new QAction(("Save &as.."), this);
    QAction *quitaction=new QAction(("&Quit"), this);

    menu->addAction(openaction);
    menu->addAction(saveaction);
    menu->addAction(saveasaction);
    menu->addSeparator();
    menu->addAction(quitaction);
}

void MainWindow::setupViewMenu(QMenu *menu){

    QAction *colorbaraction=new QAction(("&Color information bar"), this);
    QAction *histogramaction=new QAction(("&Histogram"), this);

    menu->addAction(colorbaraction);
    menu->addAction(histogramaction);

}

void MainWindow::setupImageMenu(QMenu *menu){
    QAction *crop=new QAction(("&Crop"), this);
    QAction *blur=new QAction(("&Blur"), this);
    QAction *fusion=new QAction(("&Fusion"), this);
    QAction *resize=new QAction(("&Resize"), this);

    imagemenu->addAction(crop);
    imagemenu->addAction(blur);
    imagemenu->addAction(fusion);
    imagemenu->addAction(resize);
}

void MainWindow::setupImageFilterSubMenu(QMenu *menu){
    QAction *mean=new QAction(("Mean"), this);
    QAction *gradient=new QAction(("Gradiant"), this);
    QAction *gaussian=new QAction(("Gaussian"), this);
    QAction *laplacian=new QAction(("Laplacian"), this);
    QAction *custom=new QAction(("Custom"), this);

    menu->addAction(mean);
    menu->addAction(gradient);
    menu->addAction(gaussian);
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
    colormenu->addAction(contrast);
    colormenu->addAction(convertGrey);

}

MainWindow::~MainWindow()
{
    delete ui;
}
