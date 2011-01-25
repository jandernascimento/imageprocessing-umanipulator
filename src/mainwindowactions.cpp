#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "histogram.h"
#include "dialogscale.h"
#include "dialogabout.h"
#include "dialogsetkernel.h"
#include "dialoglog.h"
#include "threadresize.h"
#include "intelligentresizing.cpp"
#include "threadprogress.h"
bool flag = false;
ImageAbstraction* copy = NULL;

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
    copy = image->copy();
    flag = true;
    label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));
    label->adjustSize();
    connect(label,SIGNAL(selected(QMouseEvent*)),this,SLOT(mouseOver(QMouseEvent*)));

    updateMenu();

}

void MainWindow::open(void){

    QFileInfo file = QFileDialog::getOpenFileName(this,
                tr("Open Image"), "", tr("Image Files (*.bmp *.jpg *.png *.pbm *.pgm *.ppm)"), 0, QFileDialog::DontUseNativeDialog);


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
            tr("Images (*.bmp *.jpg *.png *.pbm *.pgm *.ppm)"), 0, QFileDialog:: DontUseNativeDialog);

        if ( ! file.fileName().isEmpty()){
            filePath=file.absoluteFilePath();
            fileName=file.fileName();
            if (file.suffix()==NULL){
                filePath = file.absoluteFilePath() + ".png";
                fileName=file.fileName()+"png";
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

    //image->ApplyFilterGreyScale();


    dp=new DialogProgress();
    dp->show();

    threadresize *operation=new threadresize(image);
    threadprogress *progress=new threadprogress(dp->getProgress());

    //label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));

    connect(operation,SIGNAL(finished(ImageAbstraction*)),this,SLOT(updateImageReference(ImageAbstraction*)));
    connect(operation,SIGNAL(finished(ImageAbstraction*)),dp,SLOT(close()));
    connect(operation,SIGNAL(finished(ImageAbstraction*)),progress,SLOT(terminate()));
    connect(progress,SIGNAL(progresschanged(int)),dp,SLOT(setProgress(int)));

    operation->start(QThread::NormalPriority);
    progress->start(QThread::NormalPriority);

}

void MainWindow::applyBlur(){
    image->makeFilterGaussian(3,1);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applySetKernel(double* vals, int dim){
    image->ApplyConvolution(dim,vals);
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

    image=image->ApplyGradientMagnitude();

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
void MainWindow::applyUndo()
{
    if (flag)
    {
        image = copy->copy();
        //qDebug("CTRL+Z");
        label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
    }

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

    label->adjustSize();
}
void MainWindow::applyMeanFilter(){
    image->makeMeanFilter(3);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

void MainWindow::applyIntelligentResize(){
    DialogScale *ds=new DialogScale(new QString("Inteligent Resizing"),this);
    connect(ds,SIGNAL(ScaleFired(float,float)),this,SLOT(applyIntelligentResize(float,float)));
    ds->show();

}

void MainWindow::applyIntelligentResize(float width,float height){
    image=image->applySeamCarving(width,height);
    updateImageReference(image);
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
   // double* kernel = (double*)(malloc(sizeof(double)*3));
   // kernel[0] = 1;
   // kernel[1] = 0;
   // kernel[2] = -1;
    image->makeGradFilterX(3);
    //image->ApplyConvolutionLaplacian(0,kernel,'d');
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
void MainWindow::applyGradFilterY(){
    qDebug("CLICKING ON GRAD Y");
    image->makeGradFilterY(3);
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}
