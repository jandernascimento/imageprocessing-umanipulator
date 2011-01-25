#include "mainwindow.h"
#include "custom/imageabstration.h"
#include <math.h>
#include <ctime>

//******************** DECREASING METHODS *****************************//
void MainWindow::decreaseImage(int n_paths,int * energy_matrix,int * path){
    ImageAbstraction *energyImage=image->copy();
    energyImage->ApplyGradientMagnitude();
    energyImage->ApplyFilterGreyScale();
    ImageAbstraction *iatmp;

    for(int i=0;i<n_paths;i++){
        createEnergyMatrix(energy_matrix,energyImage);
        findPath(energy_matrix,path,path,0,1,energyImage);
        iatmp=image;
        image=image->scRemoveLine(path,1);
        delete(iatmp);
        iatmp=energyImage;
        energyImage=energyImage->scRemoveLine(path,1);
        delete(iatmp);
    }
    delete(energyImage);
}

void MainWindow::findPath(int * energy_matrix,int * path,int * removal_paths,int id_path,int n_paths, ImageAbstraction *ia){
    int prev_column,next_column,lin;
    int col_min_value=findMinimunValueLastLine(energy_matrix,ia);
    lin=ia->height()-1;
    path[lin]=col_min_value;

    //find the path, from mininum value up, with its neighbors
    for(int lin=ia->height()-2;lin>=0;lin--){
        calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value);

        col_min_value = findColumnMinValue(energy_matrix, lin, prev_column, col_min_value, next_column,ia);

        path[lin]=col_min_value;
    }

}

//******************** INCREASING METHODS *****************************//
void MainWindow::increaseImage(int n_paths/*,int * energy_matrix,int * path*/){
    ImageAbstraction *back_image=image->copy();

    ImageAbstraction *energyImage=image->copy();
    //energyImage->ApplyGradientMagnitude();
    //energyImage->ApplyFilterGreyScale();

    int * path=(int *) malloc(sizeof(int) * image->height() );
    //find the paths that will be removed at the copied image and duplicated at the original image
    int * removal_paths = (int *) malloc(sizeof(int) * image->height() * n_paths); //for each pixel, I need 3 more columns to store the red, green and blue's level
    for(int i=0;i<n_paths;i++){
        int * energy_matrix=(int *) malloc(sizeof(int) * (image->height()*image->width()) );

        createEnergyMatrix(energy_matrix,energyImage);
        //findPath(energy_matrix,path,removal_paths,removal_paths_red,removal_paths_green,removal_paths_blue,i,n_paths,energyImage);
        saveMatrixInFile("/home/raquel/matrixdoida.csv",removal_paths,(i+1),back_image->height());

        energyImage=energyImage->scRemoveLine(path,1);
        image=image->scRemoveLine(path,1);

        free(energy_matrix);
    }
    //saveMatrixInFile("/home/raquel/matrixdoida.csv",removal_paths,n_paths*4,back_image->height());

    /*/inserting the paths at the backuped image
    free(path);
    path=(int *) malloc(sizeof(int) * image->height() * 4);
    for(int i=n_paths-1;i>=0;i--){
        //building a array with each pixel of the matrix, that contains 4 columns (1-column of the pixel, 2-red level, 3-green level, 5-blue level)
        for(int j=0;j<back_image->height();j++){
            path[j]=removal_paths[n_paths*j+i];
            path[j+1]=removal_paths[n_paths*j+i+1];
            path[j+2]=removal_paths[n_paths*j+i+2];
            path[j+3]=removal_paths[n_paths*j+i+3];
        }
        image=image->scInsertLine(path);

    }
    */

    free(removal_paths);
    free(path);
}

/*
int * MainWindow::adjustPath(int * path, int factor,int n_lines){
    for (int i=0; i<n_lines; i++)
        path[i]=path[i]+factor;
    return path;
}
*/
/*void MainWindow::increaseImage(int n_paths,int * energy_matrix,int * path){
    ImageAbstraction *energyImage=image->copy();
    energyImage->ApplyGradientMagnitude();
    energyImage->ApplyFilterGreyScale();
    //find just one path and duplicate it n times
    createEnergyMatrix(energy_matrix,energyImage);
    findPath(energy_matrix,path,energyImage);
    for(int i=0;i<n_paths;i++){
        image=image->scInsertLine(path);
    }
}*/
/*
void MainWindow::findAndDuplicatePath(int * energy_matrix,int * path,int * values_new_path,ImageAbstraction *ia){
    int prev_column,next_column,lin;
    int col_min_value=findMinimunValueLastLine(energy_matrix,ia);
    //image->setPixel(lin,col_min_value,255,0,0);
    //energy_matrix[image->width()*lin+col_min_value]= INT_MAX;
    lin=ia->height()-1;
    path[lin]=col_min_value;
    //******* SEE IF WHEN THERE IS ONLY ONE NEIGHBOR MATTER ******** //
    calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value);
    values_new_path[lin]=(energy_matrix[ia->width()*(lin)+prev_column]+
                         energy_matrix[ia->width()*(lin)+col_min_value]+
                         energy_matrix[ia->width()*(lin)+next_column]) / 3;
    //

    //build the path, from mininum value up, with its neighbors
    for(int lin=ia->height()-2;lin>=0;lin--){
        calculatePrevAndNextColumn(&prev_column,&next_column,col_min_value);
        col_min_value = findColumnMinValue(energy_matrix, lin, col_min_value, prev_column, next_column,ia);

        //image->setPixel(lin,col_min_value,255,0,0);
        //energy_matrix[image->width()*lin+col_min_value]= INT_MAX;
        path[lin]=col_min_value;
        values_new_path[lin]=(energy_matrix[ia->width()*(lin)+prev_column]+
                             energy_matrix[ia->width()*(lin)+col_min_value]+
                             energy_matrix[ia->width()*(lin)+next_column]) / 3;
    }
}
*/

//****************** GENERAL METHODS ****************************//
//reference: http://en.wikipedia.org/wiki/Seam_carving
void MainWindow::applySeamCarving(float width,float height){    
    int n_w_paths,n_h_paths;
    int * path=(int *) malloc(sizeof(int) * image->height() * image->width());
    int * energy_matrix=(int *) malloc(sizeof(int) * (image->height()*image->width()) );
    ImageAbstraction *itmp;

    //vertical seams
    if (width>1){
        /*n_w_paths=round(image->width() * (width-1));
        qDebug("SC: Width: Enlarging image: total paths:%i",n_w_paths);
        increaseImage(n_w_paths);*/
    }else if (width<1){
        n_w_paths=round(image->width() * (1-width));
        qDebug("SC: Width: Reducing image: total paths:%i",n_w_paths);
        decreaseImage(n_w_paths,energy_matrix,path);
    }

    //horizontal seams
    itmp=image;
    image=image->transposeLeftImage();
    delete(itmp);
    if (height>1){
        /*qDebug("SC: Height: Enlarging image");
        n_h_paths=round(image->height() * (height-1));
        increaseImage(n_h_paths);*/
    }else{
        n_h_paths=round(image->height() * (1-height));
        qDebug("SC: Height: Reducing image: total paths:%i",n_h_paths);
        decreaseImage(n_h_paths,energy_matrix,path);
    }
    itmp=image;
    image=image->transposeRightImage();
    delete(itmp);


    //updating the image on the screen
    this->updateImageReference(image);

    free(path);
    free(energy_matrix);
}

void MainWindow::createEnergyMatrix(int * energy_matrix,ImageAbstraction *ia){
    int line,column,current_pixel,neighbor_pixel1,neighbor_pixel2,neighbor_pixel3,prev_column,next_column,prev_line;

    //first line:
    line=0;
    for(int col = 0; col < ia->width(); col++) {
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
            energy_matrix[ia->width()*line+column]=findMinValue(current_pixel+neighbor_pixel1,
                                                                   current_pixel+neighbor_pixel2,
                                                                   current_pixel+neighbor_pixel3);

            column++;
            prev_column=column-1;
            next_column=findMinValue(column+1,column+1,ia->width()-1);
        }while(column<ia->width());

        line++;
        prev_line=line-1;
    }while(line<ia->height());
}

void MainWindow::calculatePrevAndNextColumn(int * prev_column,int * next_column,int col_min_value){
    *prev_column=col_min_value-1;
    *next_column=col_min_value+1;
    if(col_min_value==0)
        *prev_column=col_min_value;
    else if(col_min_value==image->width()-1)
        *next_column=col_min_value;
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

void MainWindow::saveMatrixInFile(char * nome_file,int * matrix,int n_cols,int n_lines){
    FILE *fp;
    fp = fopen(nome_file,"w");
    fprintf(fp, "X,");
    for (int col = 0; col < n_cols; col++)
        fprintf(fp, "%i,",col);
    for (int lin = 0; lin < n_lines; lin++){
        fprintf(fp, "\n%i,",lin);
        for (int col = 0; col < n_cols; col++) {
            int current_pixel   = matrix[n_cols*lin+col];
            fprintf(fp, "%i,", current_pixel);
        }
    }
    fclose(fp) ;
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
