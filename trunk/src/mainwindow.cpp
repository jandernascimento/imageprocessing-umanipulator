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

    this->secondToolBar=new QToolBar("File actions");
    addToolBar(secondToolBar);

    createMenu();
    showMaximized();
    label=new ImageLabel(this);
    label->setAccessibleName("label");
    label->setObjectName("label");
    label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //QScrollArea *scroll = new QScrollArea(this);
    //scroll->setWidget(label);
    //scroll->setLayoutDirection(Qt::LayoutDirectionAuto);
    //scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //label->setLayout(ui->horizontalLayout_3);
    //ui->scrollArea->setWidget(label);

    ui->scrollArea->setWidget(label);


}

void MainWindow::createMenu(void){

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
    QMenu *imagesubfilter=imagemenu->addMenu("&Filters");
    setupImageFilterSubMenu(imagesubfilter);

    //image/advanced
    imagemenu->addSeparator();
    QMenu *imagesubadvanced=imagemenu->addMenu("&Advanced");
    setupImageAdvancedSubMenu(imagesubadvanced);

    //color
    colormenu=menuBar()->addMenu("&Color");
    setupColorMenu(colormenu);

    //Help
    helpmenu=menuBar()->addMenu("&Help");
    setupHelpMenu(helpmenu);


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

    openaction->setIcon(QIcon(":open"));
    saveasaction->setIcon(QIcon(":saveas"));

    secondToolBar->addAction(openaction);
    secondToolBar->addAction(saveasaction);

    menu->addAction(openaction);
    menu->addAction(saveaction);
    menu->addAction(saveasaction);
    menu->addSeparator();
    menu->addAction(quitaction);
}

void MainWindow::setupViewMenu(QMenu *menu){

    //QAction *colorbaraction=new QAction(("&Color information bar"), this);
    QAction *histogramaction=new QAction(("&Histogram"), this);

    configureHistogram(histogramaction);

    //menu->addAction(colorbaraction);
    menu->addAction(histogramaction);

}

void MainWindow::setupImageMenu(QMenu *menu){
    //QAction *crop=new QAction(("&Crop"), this);
    QAction *crop=new QAction(("&Crop"), this);
    QAction *blur=new QAction(("&Blur"), this);
    QAction *fusion=new QAction(("&Fusion"), this);
    QAction *resize=new QAction(("&Resize"), this);

    crop->setCheckable(true);

    crop->setIcon(QIcon(":crop"));
    fusion->setIcon(QIcon(":fusion"));
    blur->setIcon(QIcon(":blur"));
    resize->setIcon(QIcon(":resize"));

    ui->mainToolBar->addAction(crop);
    ui->mainToolBar->addAction(blur);
    ui->mainToolBar->addAction(fusion);
    ui->mainToolBar->addAction(resize);

    configureFusion(fusion);
    connect(blur, SIGNAL(triggered()),this,SLOT(applyBlur()));
    connect(crop, SIGNAL(triggered()),this,SLOT(applyCrop()));
    connect(resize, SIGNAL(triggered()),this,SLOT(applyScale()));


    menu->addAction(crop);
    menu->addAction(blur);
    menu->addAction(fusion);
    menu->addAction(resize);

}

void MainWindow::setupImageFilterSubMenu(QMenu *menu){
    QAction *mean=new QAction(("Mean"), this);
    QAction *gradientX=new QAction(("Vertical Gradiant"), this);
    QAction *gradientY=new QAction(("Horizontal Gradiant"), this);
    QAction *laplacian=new QAction(("Laplacian"), this);
    QAction *LoG=new QAction(("LoG"), this);
    QAction *custom=new QAction(("Custom"), this);
    connect(custom, SIGNAL(triggered()),this,SLOT(applyBlurCustomDialog()));
    connect(mean, SIGNAL(triggered()),this,SLOT(applyMeanFilter()));
    connect(laplacian, SIGNAL(triggered()),this,SLOT(applyLaplacianFilter()));
    connect(LoG, SIGNAL(triggered()),this,SLOT(applyCustomLoG()));
    connect(gradientX, SIGNAL(triggered()),this,SLOT(applyGradFilterX()));
    connect(gradientY, SIGNAL(triggered()),this,SLOT(applyGradFilterY()));
    menu->addAction(mean);
    menu->addAction(gradientX);
    menu->addAction(gradientY);
    menu->addAction(laplacian);
    menu->addAction(LoG);
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

    QToolBar *colorToolBar=new QToolBar("Color actions");

    QAction *contrast=new QAction(("&Contrast and equalization"), this);
    QAction *convertGrey=new QAction(("Convert to &grey scale"), this);

    contrast->setIcon(QIcon(":contrast"));
    convertGrey->setIcon(QIcon(":greyscale"));

    configureContrast(contrast);
    configureGrey(convertGrey);

    menu->addAction(contrast);
    menu->addAction(convertGrey);

    colorToolBar->addAction(convertGrey);
    colorToolBar->addAction(contrast);

    addToolBar(colorToolBar);
}

void MainWindow::setupHelpMenu(QMenu *menu){

    QAction *about=new QAction(("&About"), this);

    connect(about,SIGNAL(triggered()),this,SLOT(dialogAbout()));

    menu->addAction(about);

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
  QString message=QString("RGB(%1,%2,%3) CMYK(%4, %5, %6, %7) Pixel(%8,%9)")
                  .arg(qRed(*pixel))
                  .arg(qGreen(*pixel))
                  .arg(qBlue(*pixel))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::cyan))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::magenta))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::yellow))
                  .arg(image->RGB2CMYK(x,y,ImageAbstraction::black))
                  .arg(x)
                  .arg(y);

  ui->statusBar->showMessage(message);
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


QAction* MainWindow::retrieveMenuOption(QString option,QMenu *menu){

    QList<QAction *> menulist=menu->actions();

    QListIterator<QAction *> li=QListIterator<QAction *>(menulist);

    while(li.hasNext()){

        QAction *mainmenu=li.next();

        if(mainmenu->text()==option){
              return mainmenu;
        }


    }

    return NULL;


}
