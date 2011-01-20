#include "mainwindow.h"
#include "custom/imageabstration.h"
//OLD
void MainWindow::printMatrix(int * energy_matrix){
    for (int lin = 0; lin < image->height(); lin++){
        qDebug("line: %i",lin);
        for (int col = 0; col < image->width(); col++) {
            int current_pixel   = energy_matrix[image->width()*lin+col];
            qDebug("col,%i,%i",col,current_pixel);
        }
    }
}

void MainWindow::findPaths2(void){
    int size=(image->height())*(image->width());
    int * energy_matrix=(int *) malloc(sizeof(int) * size);

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
    qDebug("2-energy matrix...");
    //printMatrix(energy_matrix);

    for(int i=0;i<100;i++)
        highlightPaths(energy_matrix);
    /*/
    qDebug("3-path...");
    for (int lin = 0; lin < image->height(); lin++)
        for (int col = 0; col < image->width(); col++) {
            int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::red,lin,col);
            qDebug("%i",current_pixel);
        }
    //*/

    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));
    free(energy_matrix);
}

//for each line, it seeks the min value and marks with the red color
void MainWindow::highlightPaths(int * energy_matrix){
    int col_min_value=0; //stores the column of the min value
    //seek the min value at last line
    int lin=image->height()-1;
    for(int col=0;col<image->width();col++)
        if (energy_matrix[image->width()*lin+col] < energy_matrix[image->width()*lin+col_min_value])
            col_min_value=col;
    image->setPixel(lin,col_min_value,255,0,0);
    energy_matrix[image->width()*lin+col_min_value]= INT_MAX;//-1;

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
