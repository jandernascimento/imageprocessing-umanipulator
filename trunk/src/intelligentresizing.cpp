#include "mainwindow.h"
#include "custom/imageabstration.h"
#include <math.h>
#include <ctime>

//******************** INCREASING METHODS *****************************//
void MainWindow::increaseImage(int n_paths){
    /*
    ImageAbstraction *back_image=image->copy();

    ImageAbstraction *energyImage=image->copy();
    energyImage->ApplyGradientMagnitude();
    energyImage->ApplyFilterGreyScale();

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
    }*/
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

    //free(removal_paths);
    //free(path);
}

