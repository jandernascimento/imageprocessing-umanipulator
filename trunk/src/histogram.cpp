#include "histogram.h"
#include "ui_histogram.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include "custom/imageabstration.h"

Histogram::Histogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::fillArrayRealValues(enum ImageAbstraction::ecolor color,int *n_colors){
    for(int i=0;i<256;i++){
        n_colors[i]=image->getColorCounter(color,i);
    }
}

void Histogram::fillArrayCalculatedValues(enum ImageAbstraction::ecolor color,int *n_colors,int max_number_pixels,int limit_y){
    //finding the relation between the real values and the size limit of the graph
    for(int i=0;i<256;i++){
        n_colors[i]=(image->getColorCounter(color,i) * limit_y) / max_number_pixels;
    }
}

void Histogram::drawAllHistograms(){
    QGraphicsScene *scene=new QGraphicsScene();
    drawHistogram(ImageAbstraction::red,scene);
    ui->graphicsView_Red->setScene(scene);
    ui->graphicsView_Red->adjustSize();

    if (image->isGrayscale()){
        ui->graphicsView_Green->setVisible(false);
        ui->graphicsView_Blue->setVisible(false);
        ui->labelBlue->setVisible(false);
        ui->labelGreen->setVisible(false);
        ui->labelRed->setText("Grey Level");
    }
    else{
        scene=new QGraphicsScene();
        drawHistogram(ImageAbstraction::green,scene);
        ui->graphicsView_Green->setScene(scene);
        ui->graphicsView_Green->adjustSize();

        scene=new QGraphicsScene();
        drawHistogram(ImageAbstraction::blue,scene);
        ui->graphicsView_Blue->setScene(scene);
        ui->graphicsView_Blue->adjustSize();
    }

    ui->gridLayoutWidget->adjustSize();

    this->setWindowTitle("Histogram");
    this->setWindowModality(Qt::ApplicationModal);
    this->adjustSize();
}

void Histogram::drawHistogram(enum ImageAbstraction::ecolor color,QGraphicsScene *scene){
    int minx=30;
    int maxx=minx+255; //x axis goes from 0 to 255
    int miny=30;
    int maxy=miny+200;
    int max_number_pixels=image->getMaxNumberColor();
    int weight_hist=1; //the interval of histogram

    //filling the array with the number os pixels for each color's level
    int n_colors[256];
    if(max_number_pixels>(maxy-miny)) //if it is greater than the limite of the chart, we have to find the relation between them, otherwise, we use the real values
        fillArrayCalculatedValues(color,n_colors,max_number_pixels,(maxy-miny));
    else
        fillArrayRealValues(color,n_colors);

    //x1,y1,x2,y2
    scene->setSceneRect( 0, 0, 300, 250 ); //window
    scene->addLine(minx,miny,minx,maxy); //y axis
    scene->addLine(minx,maxy,maxx,maxy); //x axis

    //drawing the rectangles
    int actual_x=minx;
    for(int i=0;i<256;i++){
        //x,y,w,h
        scene->addRect(actual_x,maxy-n_colors[i],weight_hist,n_colors[i]);//draw from up to bottom, from left to right
        actual_x+=weight_hist;
    }
}
