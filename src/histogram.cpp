#include "histogram.h"
#include "ui_histogram.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include "mainwindow.h"

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

void Histogram::fillArray(int *n_colors,int n_items,int maxy,int miny){
    for(int i=0;i<n_items;i++){
        n_colors[i]=(rand()%(maxy-miny));//generate a number between miny and maxy
        //qDebug("\n%i",n_colors[i]);
    }
}

void Histogram::drawHistogram(void){
    int minx=30;
    int maxx=550; //x axis goes from 0 to 255. So maxx is equivalent to 255
    int miny=30;
    int maxy=image->getMaxNumberColor();
    qDebug("max:%i",maxy);

    int maxx_hist_default=255; //the histogram goes from 0 to 255, this starts in 30
    int weight_hist_default=1; //the interval of histogram
    int weight_hist=(maxx-minx)*weight_hist_default/maxx_hist_default; //finding the interval's relation between 550 and 255

    //filling the array with the number os pixels for each color's level
    int n_columns=maxx_hist_default/weight_hist_default; //number of columns of the histogram
    int n_colors[n_columns]; //number of times that color's level appears (height of the column of the histogram). ||should be passed to the procedure by param
    //fillArray(n_colors,n_columns,maxy,miny); //fill the array with random values
    /*/
    for(int i=0;i<n_columns;i++){
        n_colors[i]=0;
        for(int j=(i*weight_hist_default);j<((i+1)*weight_hist_default);j++){
            n_colors[i]=n_colors[i];
            qDebug("%i",MainWindow.image->getColorCounter(ImageAbstraction::blue,j));
            qDebug("\n%i-%i-%i",i,j,n_colors[i]);
        }
    }
    //*/
    //qDebug("%i-%i",n_columns,weight_hist);


    QGraphicsScene *scene=new QGraphicsScene();
    //x1,y1,x2,y2
    //scene->setSceneRect( 0, 0, 598, 398 ); //window
    scene->addLine(minx,miny,minx,maxy); //y axis
    scene->addLine(minx,maxy,maxx,maxy); //x axis


    //drawing the rectangles
    int actual_x=minx;
    for(int i=0;i<n_columns;i++){
        //x,y,w,h
        scene->addRect(actual_x,maxy-n_colors[i],weight_hist,n_colors[i]);
        actual_x+=weight_hist;
    }

    ui->graphicsView->setScene(scene);
    ui->graphicsView->adjustSize();

    this->setWindowTitle("Histogram");
    this->setWindowModality(Qt::ApplicationModal);
    this->adjustSize();
}
