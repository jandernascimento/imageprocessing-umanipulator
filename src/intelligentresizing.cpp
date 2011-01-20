#include "mainwindow.h"
#include "custom/imageabstration.h"
#include <math.h>
//OLD
void MainWindow::printMatrix(int * matrix,int n_lin, int n_col){
    for (int lin = 0; lin < n_lin; lin++){
        qDebug("line: %i",lin);
        for (int col = 0; col < n_col; col++) {
            int current_pixel   = matrix[n_col*lin+col];
            qDebug("col,%i,%i",col,current_pixel);
        }
    }
}

int MainWindow::calculateNumberPaths(int size, float perc){
    return (round(size * perc));
}

void MainWindow::applySeamCarving(float width,float height){    
    int n_vertical_paths=calculateNumberPaths(image->width(),width);
    qDebug("width %i - %f% = %i",image->width(),width*100,n_vertical_paths);

    //height=1;
    //qDebug("height: %f",height);

    int * energy_matrix=(int *) malloc(sizeof(int) * (image->height()*image->width()) );
    int * vertical_paths=(int *) malloc(sizeof(int) * (image->height() * n_vertical_paths));

    /*/printing the initial matrix of the image
    qDebug("1-initial values...");
    for (int lin = 0; lin < image->height(); lin++){
        qDebug("line: %i",lin);
        for (int col = 0; col < image->width(); col++) {
            int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,lin,col);
            qDebug("col,%i,%i",col, current_pixel);
        }
    }
    //*/

    createEnergyMatrix(energy_matrix);
    //qDebug("2-energy matrix...");
    //printMatrix(energy_matrix);

    for(int i=0;i<n_vertical_paths;i++)
        findPaths(energy_matrix,vertical_paths,i,n_vertical_paths);
    /*/
    qDebug("3-path...");
    for (int lin = 0; lin < image->height(); lin++)
        for (int col = 0; col < image->width(); col++) {
            int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::red,lin,col);
            qDebug("%i",current_pixel);
        }
    //*/

    //printMatrix(vertical_paths,image->height(),n_vertical_paths);
    image=image->scRemoveLine(vertical_paths,image->height(),n_vertical_paths);

    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));
    label->adjustSize();
    free(energy_matrix);
    free(vertical_paths);
}

//for each line, it seeks the min value and marks with the red color
void MainWindow::findPaths(int * energy_matrix,int * vertical_paths,int id_path,int n_paths){
    int col_min_value=0; //stores the column of the min value
    //seek the min value at last line
    int lin=image->height()-1;
    for(int col=0;col<image->width();col++)
        if (energy_matrix[image->width()*lin+col] < energy_matrix[image->width()*lin+col_min_value])
            col_min_value=col;
    image->setPixel(lin,col_min_value,255,0,0);
    energy_matrix[image->width()*lin+col_min_value]= INT_MAX;//-1;
    vertical_paths[n_paths*lin+id_path]=col_min_value;

    //build the path, from mininum value up, with its neighbors
    int prev_column,next_column;
    for(int lin=image->height()-2;lin>=0;lin--){
        prev_column=col_min_value-1;
        next_column=col_min_value+1;
        if(col_min_value==0)
            prev_column=col_min_value;
        else if(col_min_value==image->width()-1)
            next_column=col_min_value;
        //
        col_min_value = findColumnMinValue(energy_matrix, lin, col_min_value, prev_column, next_column);


        image->setPixel(lin,col_min_value,255,0,0);
        energy_matrix[image->width()*lin+col_min_value]= INT_MAX;//-1;
        vertical_paths[n_paths*lin+id_path]=col_min_value;
    }

}

void MainWindow::createEnergyMatrix(int * energy_matrix){
    int line,column,current_pixel,neighbor_pixel1,neighbor_pixel2,neighbor_pixel3,prev_column,next_column,prev_line;

    //reference: http://en.wikipedia.org/wiki/Seam_carving
    //first, find vertical seams

    //first line:
    line=0;
    for(int col = 0; col < image->width(); col++) {
        current_pixel = image->getPixelColorIntensity(ImageAbstraction::blue,line,col);
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
            current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,line,column);
            neighbor_pixel1 = energy_matrix[image->width()*prev_line+prev_column];
            neighbor_pixel2 = energy_matrix[image->width()*prev_line+column];
            neighbor_pixel3 = energy_matrix[image->width()*prev_line+next_column];
            ///
            energy_matrix[image->width()*line+column]=findMaxValue(current_pixel+neighbor_pixel2,
                                                                   current_pixel+neighbor_pixel1,
                                                                   current_pixel+neighbor_pixel3);
            //*/

            //energy_matrix[image->width()*line+column]=current_pixel+neighbor_pixel2+neighbor_pixel1+neighbor_pixel3;

            column++;
            prev_column=column-1;
            next_column=findMinValue(column+1,column+1,image->width()-1);
        }while(column<image->width());

        line++;
        prev_line=line-1;
    }while(line<image->height());
}

int MainWindow::findMinValue(int value1,int value2, int value3){
    if (value1<=value2 && value1<=value3)
        return value1;
    else if (value2<=value3)
        return value2;
    else
        return value3;
}

int MainWindow::findMaxValue(int value1,int value2, int value3){
    if (value1>=value2 && value1>=value3)
        return value1;
    else if (value2>=value3)
        return value2;
    else
        return value3;
}

int MainWindow::findColumnMinValue(int * energy_matrix,int lin,int prev_col,int col,int next_col){
    if      (energy_matrix[image->width()*lin+prev_col] <= energy_matrix[image->width()*lin+col]
            &&
            energy_matrix[image->width()*lin+prev_col] <= energy_matrix[image->width()*lin+next_col])
        return prev_col;
    else if (energy_matrix[image->width()*lin+col] <= energy_matrix[image->width()*lin+next_col])
        return col;
    else
        return next_col;
}

int MainWindow::findColumnMaxValue(int * energy_matrix,int lin,int prev_col,int col,int next_col){
    if      (energy_matrix[image->width()*lin+prev_col] >= energy_matrix[image->width()*lin+col]
            &&
            energy_matrix[image->width()*lin+prev_col] >= energy_matrix[image->width()*lin+next_col])
        return prev_col;
    else if (energy_matrix[image->width()*lin+col] >= energy_matrix[image->width()*lin+next_col])
        return col;
    else
        return next_col;
}
