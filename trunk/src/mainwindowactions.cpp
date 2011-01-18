#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "histogram.h"
#include "dialogscale.h"
#include "dialogabout.h"
#include "dialogsetkernel.h"
#include "dialoglog.h"

void MainWindow::findPaths(void){
    qDebug("heigth: %i, width: %i",image->height(),image->width());

    int size=(image->height())*(image->width());
    int * energy_matrix=(int *) malloc(sizeof(int) * size);//image->height*image->width));

    //reference: http://en.wikipedia.org/wiki/Seam_carving
    //first, find vertical seams

    //first line:
    for(int col = 0; col < image->width(); col++) {
        int lin=0;
        int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin);
        energy_matrix[image->width()*lin+col]=current_pixel;
    }
    //first col:
    for(int lin = 1; lin < image->height(); lin++) {
        int col=0;
        int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin);
        int neighbor_pixel2 = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin-1);
        int neighbor_pixel3 = image->getPixelColorIntensity(ImageAbstraction::blue,col+1,lin-1);
        energy_matrix[image->width()*lin+col]=findMinValue(current_pixel+neighbor_pixel2,
                                                           current_pixel+neighbor_pixel2,
                                                           current_pixel+neighbor_pixel3);
    }
    //last col:
    for(int lin = 1; lin < image->height(); lin++) {
        int col=image->width()-1;
        int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin);
        int neighbor_pixel1 = image->getPixelColorIntensity(ImageAbstraction::blue,col-1,lin-1);
        int neighbor_pixel2 = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin-1);
        energy_matrix[image->width()*lin+col]=findMinValue(current_pixel+neighbor_pixel1,
                                                           current_pixel+neighbor_pixel2,
                                                           current_pixel+neighbor_pixel2);
    }
    //next lines:
    for (int lin = 1; lin < image->height(); lin++)
        for (int col = 0; col < image->width(); col++) {
            int current_pixel   = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin);
            int neighbor_pixel1 = image->getPixelColorIntensity(ImageAbstraction::blue,col-1,lin-1);
            int neighbor_pixel2 = image->getPixelColorIntensity(ImageAbstraction::blue,col,lin-1);
            int neighbor_pixel3 = image->getPixelColorIntensity(ImageAbstraction::blue,col+1,lin-1);
            energy_matrix[image->width()*lin+col]=findMinValue(current_pixel+neighbor_pixel1,
                                                               current_pixel+neighbor_pixel2,
                                                               current_pixel+neighbor_pixel3);

        }

    free(energy_matrix);
    qDebug("finished");
}

int MainWindow::findMinValue(int value1,int value2, int value3){
    if (value1>=value2 && value1>=value3)
        return value1;
    else if (value2>=value3)
        return value2;
    else
        return value3;
}

void MainWindow::detectEdges(void){

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

void MainWindow::histogram(void){
    if (fileName == NULL)
        QMessageBox::warning(this, tr("Warning"), tr("There is no file in use."));
    else{
        image->UpdateColorRange();
        Histogram *histowin=new Histogram(this);
        histowin->image = image;
        histowin->drawAllHistograms();
        histowin->show();
    }
}

void MainWindow::quit(void){
    QCoreApplication::exit();
}

void MainWindow::openFile(void){
    image=new ImageAbstraction(filePath);
    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));

    connect(label,SIGNAL(selected(QMouseEvent*)),this,SLOT(mouseOver(QMouseEvent*)));
}

void MainWindow::open(void){

    QFileInfo file = QFileDialog::getOpenFileName(this,
                tr("Open Image"), "", tr("Image Files (*.bmp *.gif *.jpg *.png *.pbm *.pgm *.ppm)"), 0, QFileDialog::DontUseNativeDialog);


    QString previousName = fileName;
    fileName=file.fileName();

    if (!fileName.isEmpty()){
        filePath=file.absoluteFilePath();
        openFile();
    }
    else
        fileName=previousName;
}

void MainWindow::saveImage(){
    this->image->save(filePath,0,-1);
}

void MainWindow::save(void){
    if (fileName == NULL)
        QMessageBox::warning(this, tr("Warning"), tr("There is no file in use."));
    else{
        this->saveImage();
    }

}

void MainWindow::saveas(void){
    if (fileName == NULL)
        QMessageBox::warning(this, tr("Warning"), tr("There is no file in use."));
    else{
        QFileInfo file=QFileDialog::getSaveFileName(this, tr("Save Image"), "",
            tr("Images (*.bmp *.gif *.jpg *.png *.pbm *.pgm *.ppm)"), 0, QFileDialog:: DontUseNativeDialog);

        if ( ! file.fileName().isEmpty()){
            filePath=file.absoluteFilePath();
            fileName=file.fileName();
            if (file.suffix()==NULL){
                filePath = file.absoluteFilePath() + ".jpg";
                fileName=file.fileName()+"jpg";
            }

            save();
            openFile();
        }
    }
}

void MainWindow::dialogContrast(void){

    mDialogContrast=new DialogContrast();

    mDialogContrast->setMinMax(
            image->getMinColorValue(ImageAbstraction::blue),
            image->getMaxColorValue(ImageAbstraction::blue)
            );
    mDialogContrast->update();

   mDialogContrast->show();
   connect(mDialogContrast,
           SIGNAL(constrastChanged(int,int)),
           this,
           SLOT(applyContrast(int,int)));

}

void MainWindow::dialogAbout(void){
    //About
    DialogAbout *da=new DialogAbout();
    da->show();

}


void MainWindow::applyContrast(int newmin,int newmax){

    image->ApplyFilterContrast(newmin,newmax);

    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));

}

void MainWindow::applyFusion(QString path, float percentage,int x, int y){

    ImageAbstraction *externalImage=new ImageAbstraction(path,0);

    this->image->ApplyFilterFusion(externalImage,percentage,x,y);

    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));
}

void MainWindow::applyCrop(){


    QAction *crop=retrieveMenuOption(QString("&Crop"),imagemenu);

    if(crop->isChecked()){
        qDebug("Activating Crop");
        connect(label,SIGNAL(areaselected(int,int,int,int)),this,SLOT(applyCrop(int,int,int,int)));
        label->setRubberband(true);
        label->setCursor(Qt::CrossCursor);
    }else{
        qDebug("Deactivating Crop");
        disconnect(label,SIGNAL(areaselected(int,int,int,int)),this,SLOT(applyCrop(int,int,int,int)));
        label->setRubberband(false);
        label->setCursor(Qt::ArrowCursor);
    }


}

void MainWindow::applyGrey(){

    image->ApplyFilterGreyScale();

    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));

}

void MainWindow::applyBlur(){
    image->makeFilterGaussian(3,1);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applySetKernel(double* vals, int dim){
    image->ApplyConvolution(dim,vals,'X');
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

void MainWindow::applyCustomLoG(){
    dialogLoG *dl = new dialogLoG();
    connect(dl, SIGNAL(log(int, double)),this,SLOT(applyLoG(int, double)));
    dl->show();
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applyLoG(int dim, double sig){
    image->makeLoG(dim,sig);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applyTEMP(){
    image->makeLaplacianFilter(5);
    int r,g,b;
    for (int i=0;i<image->height();++i)
        for (int j=0; j<image->width();++j)
        {
            r = image->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g = image->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b = image->getPixelColorIntensity(ImageAbstraction::blue,i,j);
            if ((r+g+b)/3>128)
                image->setPixel(i,j,0,0,0);
            else
                image->setPixel(i,j,255,255,255);

        }
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

void MainWindow::applyBlurCustom(int dim, bool r1,bool r2,bool r3){
    if (r1)
        image->makeFilterGaussian(dim,1.0);
    if (r2)
        image->makeMeanFilter(dim);
    if (r3)
    {
        dialogSetKernel *dsk = new dialogSetKernel();
        //dsk->childAt(0,0)->childAt(0,0)->setVisible(true);
        dsk->setSize(dim);
        dsk->show();
        connect(dsk, SIGNAL(setKernel(double*,int)),this,SLOT(applySetKernel(double*,int)));
        label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
    }
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applyBlurCustomDialog(){

    dialogCustom *dc=new dialogCustom();
    dc->show();
    connect(dc, SIGNAL(custom(int,bool,bool,bool)),this, SLOT(applyBlurCustom(int,bool,bool,bool)));
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::dialogFusion(void){

    dialogfusion *df=new dialogfusion();

    connect(df, SIGNAL(fusion(QString,float ,int , int )),this, SLOT(applyFusion(QString,float ,int , int)));

    df->show();

}

void MainWindow::applyCrop(int startx,int starty,int endx,int endy){

    image=image->ApplyCrop(startx,starty,endx,endy);

    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));

}
void MainWindow::applyMeanFilter(){
    qDebug("CLICKING ON MEAN");
    image->makeMeanFilter(3);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

void MainWindow::applyScale(){
    DialogScale *ds=new DialogScale(this);
    connect(ds,SIGNAL(ScaleFired(float,float)),this,SLOT(applyScale(float,float)));
    ds->show();
}

void MainWindow::applyScale(float width,float height){
   this->image=image->ApplyScale(width,height);
   label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

void MainWindow::applyLaplacianFilter(){
    qDebug("CLICKING ON LAPLACIAN");
    image->makeLaplacianFilter(3);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applyGradFilterX(){
    qDebug("CLICKING ON GRAD X");
    image->makeGradFilterX(3,0);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applyGradFilterY(){
    qDebug("CLICKING ON GRAD Y");
    image->makeGradFilterY(3,0);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
