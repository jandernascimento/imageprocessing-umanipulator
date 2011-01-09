#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include "dialogcontrast.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //drawing Histogram
    //w.open();

    QGraphicsScene scene;
    int minx=30;
    int maxx=550; //x axis goes from 0 to 255. So maxx is equivalent to 255
    int miny=30;
    int maxy=350;
    int maxx_hist_default=255; //the histogram goes from 0 to 255
    int weight_hist_default=25; //the interval of histogram
    int n_columns=maxx_hist_default/weight_hist_default; //number of columns of the histogram
    int n_colors[13]={10,20,30,100,200,300,300,200,100,30,20,10,5};//number of times that level of the color appears (height of the column of the histogram). ||should be passed to the procedure by param

    int weight_hist=maxx*weight_hist_default/maxx_hist_default; //finding the interval's relation between 550 and 255

    //x1,y1,x2,y2
    scene.setSceneRect( 0, 0, 598, 398 ); //window
    scene.addLine(minx,miny,minx,maxy); //y axis
    scene.addLine(minx,maxy,maxx,maxy); //x axis

    //drawing the rectangles
    int actual_x=minx;
    int item=0;
    while(actual_x<maxx){
        //x,y,w,h
        scene.addRect(actual_x,maxy-n_colors[item],weight_hist,n_colors[item]);
        actual_x+=weight_hist;
        item++;
    }

    /*QRectF t;
    t.setX(10);
    t.setY(10);
    t.setWidth(50);
    t.setHeight(50);
    scene.addRect(t);*/

    QGraphicsView view( &scene );
    view.show();
    //



    //DialogContrast dialog;// = new DialogContrast();

    //dialog.show();
    //dialog.setWindow(&w);

    return a.exec();
}
