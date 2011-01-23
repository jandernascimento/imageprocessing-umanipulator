#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "histogram.h"
#include "dialogscale.h"
#include "dialogabout.h"
#include "dialogsetkernel.h"
#include "dialoglog.h"
#include "intelligentresizing.cpp"

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
    label->adjustSize();
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

    ImageAbstraction *k1=new ImageAbstraction(image->copy(0,0,image->width(),image->height()));
    ImageAbstraction *k2=new ImageAbstraction(image->copy(0,0,image->width(),image->height()));

    k1->makeGradFilterX(5,0);
    k2->makeGradFilterY(5,0);


    int r1,g1,b1,r2,g2,b2,r,g,b;
    for (int i=0;i<image->height();++i)
        for (int j=0; j<image->width();++j)
        {
            r1 = k1->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g1 = k1->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b1 = k1->getPixelColorIntensity(ImageAbstraction::blue,i,j);

            r2 = k2->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g2 = k2->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b2 = k2->getPixelColorIntensity(ImageAbstraction::blue,i,j);
            r=sqrt(pow(r1-r2,2)+pow(r2-r1,2));
            g=sqrt(pow(g1-g2,2)+pow(g2-g1,2));
            b=sqrt(pow(b1-b2,2)+pow(b2-b1,2));
           // r=abs(r1)+abs(r2);
            //g=abs(g1)+abs(g2);
            //b=abs(b1)+abs(b2);

            //if ((r+g+b)/3>128)
                //image->setPixel(i,j,0,0,0);
            //    image->setPixel(i,j,255,255,255);
            //else
                //image->setPixel(i,j,255,255,255);
            //int SUM;
            //if((r+g+b)>255) SUM=255;
            //else SUM=0;


            image->setPixel(i,j,r,g,b);

        }

    /*
    int r,g,b;
    for (int i=0;i<image->height();++i)
        for (int j=0; j<image->width();++j)
        {
            r = image->getPixelColorIntensity(ImageAbstraction::red,i,j);
            g = image->getPixelColorIntensity(ImageAbstraction::green,i,j);
            b = image->getPixelColorIntensity(ImageAbstraction::blue,i,j);
            if ((r+g+b)/3>128)
                //image->setPixel(i,j,0,0,0);
                image->setPixel(i,j,255,255,255);
            else
                //image->setPixel(i,j,255,255,255);
                image->setPixel(i,j,0,0,0);

        }
        */

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

void MainWindow::applyIntelligentResize(){
    DialogScale *ds=new DialogScale(this);
    connect(ds,SIGNAL(ScaleFired(float,float)),this,SLOT(applyIntelligentResize(float,float)));
    ds->show();
}

void MainWindow::applyIntelligentResize(float width,float height){
    applySeamCarving(width,height);
}


void MainWindow::applyScale(){
    DialogScale *ds=new DialogScale(this);
    connect(ds,SIGNAL(ScaleFired(float,float)),this,SLOT(applyScale(float,float)));
    ds->show();
}

void MainWindow::applyScale(float width,float height){

    int *ma=(int*)malloc(sizeof(int)*1*this->image->height());

    for(int x=0;x<this->image->height();x++){
        ma[x]=50;
    }

    this->image=image->ApplyScale(width,height);//image->scRemoveLine(ma,this->image->height(),1);//
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
    label->adjustSize();
}

void MainWindow::applyLaplacianFilter(){
    qDebug("CLICKING ON LAPLACIAN");
    //image->ApplyFilterGreyScale();
    image->makeLaplacianFilter(5);
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
