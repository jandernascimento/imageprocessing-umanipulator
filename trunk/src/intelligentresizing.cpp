#include "mainwindow.h"
#include "custom/imageabstration.h"
#include <math.h>
#include <ctime>

int MainWindow::calculateNumberPaths(int size, float perc){
    return size*(1-perc); //(size-(round(size * perc)));
}

void MainWindow::decreaseImage(int n_paths){
    int * paths=(int *) malloc(sizeof(int) * image->height() );
    int * energy_matrix=(int *) malloc(sizeof(int) * (image->height()*image->width()) );
    ImageAbstraction *energyImage=image->copy();
    energyImage->ApplyGradientMagnitude();
    energyImage->ApplyFilterGreyScale();

    for(int i=0;i<n_paths;i++){
        createEnergyMatrix(energy_matrix,energyImage);
        findPaths(energy_matrix,paths,0,1,energyImage);
        image=image->scRemoveLine(paths,image->height(),1);
        energyImage=energyImage->scRemoveLine(paths,energyImage->height(),1);
    }
    free(energy_matrix);
    free(paths);
}

void MainWindow::increaseImage(int n_paths){
    int * path=(int *) malloc(sizeof(int) * image->height() );
    int * values_new_path=(int *) malloc(sizeof(int) * image->height() );
    for(int i=0;i<n_paths;i++){
        int * energy_matrix=(int *) malloc(sizeof(int) * (image->height()*image->width()) );
        ImageAbstraction *energyImage=image->copy();
        energyImage->ApplyGradientMagnitude();
        createEnergyMatrix(energy_matrix,energyImage);
        findAndDuplicatePath(energy_matrix,path,values_new_path,energyImage);
        //image=image->scRemoveLine(path,image->height(),1);
        printMatrix(values_new_path,image->height(),1);
        free(energy_matrix);
    }
    free(path);
    free(values_new_path);
}

//reference: http://en.wikipedia.org/wiki/Seam_carving
void MainWindow::applySeamCarving(float width,float height){    
    int n_paths;
    unsigned t0=clock();
    n_paths=calculateNumberPaths(image->width(),width);
    decreaseImage(n_paths);

    //horizontal seams
    n_paths=calculateNumberPaths(image->height(),height);
    image=image->transposeLeftImage();
    decreaseImage(n_paths);
    image=image->transposeRightImage();

    //updating the image on the screen
    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));
    label->adjustSize();
    qDebug("Total time:%i",clock()-t0);
}

void MainWindow::calculatePrevAndNextColumn(int * prev_column,int * next_column,int col_min_value){
    *prev_column=col_min_value-1;
    *next_column=col_min_value+1;
    if(col_min_value==0)
        *prev_column=col_min_value;
    else if(col_min_value==image->width()-1)
        *next_column=col_min_value;
}

void MainWindow::findPaths(int * energy_matrix,int * vertical_paths,int id_path,int n_paths,ImageAbstraction *ia){
    int col_min_value=findMinimunValueLastLine(energy_matrix,ia);
    //image->setPixel(lin,col_min_value,255,0,0);
    //energy_matrix[image->width()*lin+col_min_value]= INT_MAX;
    vertical_paths[n_paths*(image->height()-1)+id_path]=col_min_value;
    //

    //build the path, from mininum value up, with its neighbors
    int prev_column,next_column;
    for(int lin=image->height()-2;lin>=0;lin--){
        calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value);

        col_min_value = findColumnMinValue(energy_matrix, lin, col_min_value, prev_column, next_column,ia);

        //image->setPixel(lin,col_min_value,255,0,0);
        //energy_matrix[image->width()*lin+col_min_value]= INT_MAX;
        vertical_paths[n_paths*lin+id_path]=col_min_value;
    }

}

//seek the min value at last line
int MainWindow::findMinimunValueLastLine(int * energy_matrix,ImageAbstraction *ia){
    int col_min_value=0;
    int lin=ia->height()-1;
    for(int col=0;col<ia->width();col++)
        if (energy_matrix[ia->width()*lin+col] < energy_matrix[ia->width()*lin+col_min_value])
            col_min_value=col;
    return col_min_value;
}

void MainWindow::findAndDuplicatePath(int * energy_matrix,int * path,int * values_new_path,ImageAbstraction *ia){
    int prev_column,next_column,lin;
    int col_min_value=findMinimunValueLastLine(energy_matrix,ia);
    //image->setPixel(lin,col_min_value,255,0,0);

    //energy_matrix[image->width()*lin+col_min_value]= INT_MAX;
    calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value);
    lin=image->height()-1;
    path[lin]=col_min_value;
    //******* SEE IF WHEN THERE IS ONLY ONE NEIGHBOR MATTER ******** //
    values_new_path[lin]=(energy_matrix[image->width()*(lin)+prev_column]+
                         energy_matrix[image->width()*(lin)+col_min_value]+
                         energy_matrix[image->width()*(lin)+next_column]) / 3;
    //

    //build the path, from mininum value up, with its neighbors
    for(int lin=image->height()-2;lin>=0;lin--){
        calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value);
        col_min_value = findColumnMinValue(energy_matrix, lin, col_min_value, prev_column, next_column,ia);

        //image->setPixel(lin,col_min_value,255,0,0);
        //energy_matrix[image->width()*lin+col_min_value]= INT_MAX;
        path[lin]=col_min_value;
        values_new_path[lin]=(energy_matrix[image->width()*(lin)+prev_column]+
                             energy_matrix[image->width()*(lin)+col_min_value]+
                             energy_matrix[image->width()*(lin)+next_column]) / 3;
    }
}

void MainWindow::createEnergyMatrix(int * energy_matrix,ImageAbstraction *ia){
    int line,column,current_pixel,neighbor_pixel1,neighbor_pixel2,neighbor_pixel3,prev_column,next_column,prev_line;

    //first line:
    line=0;
    for(int col = 0; col < image->width(); col++) {
        current_pixel = ia->getPixelColorIntensity(ImageAbstraction::blue,line,col);
        energy_matrix[ia->width()*line+col]=current_pixel;
    }
    //*/

    //next lines:
    line=1;
    prev_line=line-1;
    do{
        column=0;
        prev_column=column;
        next_column=findMinValue(column+1,column+1,ia->width()-1);
        do{
            current_pixel   = ia->getPixelColorIntensity(ImageAbstraction::blue,line,column);
            neighbor_pixel1 = energy_matrix[ia->width()*prev_line+prev_column];
            neighbor_pixel2 = energy_matrix[ia->width()*prev_line+column];
            neighbor_pixel3 = energy_matrix[ia->width()*prev_line+next_column];
            energy_matrix[ia->width()*line+column]=findMinValue(current_pixel+neighbor_pixel2,
                                                                   current_pixel+neighbor_pixel1,
                                                                   current_pixel+neighbor_pixel3);

            column++;
            prev_column=column-1;
            next_column=findMinValue(column+1,column+1,image->width()-1);
        }while(column<ia->width());

        line++;
        prev_line=line-1;
    }while(line<ia->height());
}

int MainWindow::findMinValue(int value1,int value2, int value3){
    if (value1<=value2 && value1<=value3)
        return value1;
    else if (value2<=value3)
        return value2;
    else
        return value3;
}

int MainWindow::findColumnMinValue(int * energy_matrix,int lin,int prev_col,int col,int next_col,ImageAbstraction *ia){
    if      (energy_matrix[ia->width()*lin+prev_col] <= energy_matrix[ia->width()*lin+col]
            &&
            energy_matrix[ia->width()*lin+prev_col] <= energy_matrix[ia->width()*lin+next_col])
        return prev_col;
    else if (energy_matrix[ia->width()*lin+col] <= energy_matrix[ia->width()*lin+next_col])
        return col;
    else
        return next_col;
}

void MainWindow::printMatrix(int * matrix,int n_lin, int n_col){
    for (int col = 0; col < n_col; col++) {
        qDebug("col: %i",col);
        for (int lin = 0; lin < n_lin; lin++){
            int current_pixel   = matrix[n_col*lin+col];
            qDebug("lin,%i,%i",lin,current_pixel);
        }
    }
}


/*/ saving in a file
FILE *fp;
fp = fopen("/home/raquel/original.csv","w");
fprintf(fp, "X,");
for (int col = 0; col < image->width(); col++)
    fprintf(fp, "%i,",col);
for (int lin = 0; lin < image->height(); lin++){
    fprintf(fp, "\n%i,",lin);
    for (int col = 0; col < image->width(); col++) {
        int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,lin,col);
        fprintf(fp, "%i,", current_pixel);
    }
}
fclose(fp) ;
//*/

/*/ saving in a file
FILE *fp2;
fp2 = fopen("/home/raquel/inspect.csv","w");
fprintf(fp2, "X,");
for (int col = 0; col < image->width(); col++)
    fprintf(fp2, "%i,",col);
for (int lin = 0; lin < image->height(); lin++){
    fprintf(fp2, "\n%i,",lin);
    for (int col = 0; col < image->width(); col++) {
        int current_pixel   = energy_matrix[image->width()*lin+col];
        fprintf(fp2, "%i,", current_pixel);
    }
}
fclose(fp2) ;
//*/
