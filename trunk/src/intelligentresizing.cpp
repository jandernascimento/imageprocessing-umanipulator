#include "mainwindow.h"
#include "custom/imageabstration.h"

/*intelligentresizing::intelligentresizing()
{
}*/

void MainWindow::findPaths2(void){
    createEnergyMatrix();
    //highlightPath();
}

/*void MainWindow::highlightPath(void){

}*/

void MainWindow::createEnergyMatrix(void){
    int line,column,current_pixel,neighbor_pixel1,neighbor_pixel2,neighbor_pixel3,prev_column,next_column,prev_line;
    int size=(image->height())*(image->width());
    int * energy_matrix=(int *) malloc(sizeof(int) * size);


    //printing the initial matrix of the image
    for (int lin = 0; lin < image->height(); lin++)
        for (int col = 0; col < image->width(); col++) {
            current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin);
            qDebug("%i",current_pixel);
        }
    //

    //reference: http://en.wikipedia.org/wiki/Seam_carving
    //first, find vertical seams

    //first line:
    line=0;
    for(int col = 0; col < image->width(); col++) {
        current_pixel = image->getPixelColorIntensity(ImageAbstraction::blue,col,line);
        energy_matrix[image->width()*line+col]=current_pixel;
    }

    //next lines:
    line=1;
    prev_line=line-1;
    do{
        column=0;
        prev_column=column;
        next_column=findMinValue(column+1,column+1,image->width()-1);
        do{
            current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,column,line);
            neighbor_pixel1 = energy_matrix[image->width()*prev_line+prev_column];
            neighbor_pixel2 = energy_matrix[image->width()*prev_line+column];
            neighbor_pixel3 = energy_matrix[image->width()*prev_line+next_column];
            energy_matrix[image->width()*line+column]=findMinValue(current_pixel+neighbor_pixel1,
                                                                   current_pixel+neighbor_pixel2,
                                                                   current_pixel+neighbor_pixel3);
            column++;
            prev_column=column-1;
            next_column=findMinValue(column+1,column+1,image->width()-1);
        }while(column<image->width());

        line++;
        prev_line=line-1;
    }while(line<image->height());
    //

    //printing the initial matrix of the image
    qDebug("...");
    for (int lin = 0; lin < image->height(); lin++)
        for (int col = 0; col < image->width(); col++) {
            current_pixel   = energy_matrix[image->width()*lin+col];
            qDebug("%i",current_pixel);
        }
    //

    free(energy_matrix);
}

int MainWindow::findMinValue(int value1,int value2, int value3){
    if (value1<=value2 && value1<=value3)
        return value1;
    else if (value2<=value3)
        return value2;
    else
        return value3;
}

void MainWindow::detectEdges2(void){

    int size=3;
    double *gx=(double *)malloc(sizeof(double)*(size*size));
    double *gy=(double *)malloc(sizeof(double)*(size*size));

    //[line+col]
    gx[size*0+0]=-1;  gx[size*0+1]=0;  gx[size*0+2]=1;
    gx[size*1+0]=-2;  gx[size*1+1]=0;  gx[size*1+2]=2;
    gx[size*2+0]=-1;  gx[size*2+1]=0;  gx[size*2+2]=1;

    gy[size*0+0]= 1; gy[size*0+1]= 2; gy[size*0+2]= 1;
    gy[size*1+0]= 0; gy[size*1+1]= 0; gy[size*1+2]= 0;
    gy[size*2+0]=-1; gy[size*2+1]=-2; gy[size*2+2]=-1;

    image->ApplyConvolution(size,gx,'c'); //this 'c' does not matter
    image->ApplyConvolution(size,gy,'c');

    for (int j = 0; j < image->height(); j++) {
      for (int i = 0; i < image->width(); i++) {

          QRgb *pixel=image->getPixel(j,i);

          int col=image->getPixelColorIntensity(ImageAbstraction::blue,j,i);

          if(col<122 || col >129)
            *pixel=qRgba(255,255,255,255);
          else
            *pixel=qRgba(0,0,0,255);

      }
    }

    free(gy);
    free(gx);

    //updating the image in the interface
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

