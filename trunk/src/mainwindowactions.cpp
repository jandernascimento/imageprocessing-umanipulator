#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "histogram.h"
#include "dialogscale.h"
#include "dialogabout.h"
#include "dialogsetkernel.h"
#include "dialoglog.h"
#include "threadresize.h"
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
    DialogAbout *da=new DialogAbout();
    da->show();
}


void MainWindow::applyContrast(int newmin,int newmax){
    image->ApplyFilterContrast(newmin,newmax);
    updateImageReference(this->image);
}

void MainWindow::applyFusion(QString path, float percentage,int x, int y){

    ImageAbstraction *externalImage=new ImageAbstraction(path,0);

    image->ApplyFilterFusion(externalImage,percentage,x,y);

    delete(externalImage);

    updateImageReference(this->image);
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
    updateImageReference(this->image);
}

void MainWindow::applyBlur(){
    image->makeFilterGaussian(3,1);
    updateImageReference(this->image);
}
void MainWindow::applySetKernel(double* vals, int dim){
    image->ApplyConvolution(dim,vals);
    updateImageReference(this->image);
}

void MainWindow::applyCustomLoG(){
    dialogLoG *dl = new dialogLoG();
    connect(dl, SIGNAL(log(int, double)),this,SLOT(applyLoG(int, double)));
    dl->show();
    updateImageReference(this->image);
}
void MainWindow::applyLoG(int dim, double sig){
    image->makeLoG(dim,sig);
    updateImageReference(this->image);
}
void MainWindow::applyTEMP(){
    updateImageReference(image->ApplyGradientMagnitude());
}

void MainWindow::applyBlurCustom(int dim, bool r1,bool r2,bool r3){
    if (r1)
        image->makeFilterGaussian(dim,1.0);
    if (r2)
        image->makeMeanFilter(dim);
    if (r3){
        dialogSetKernel *dsk = new dialogSetKernel();
        dsk->setSize(dim);
        dsk->show();
        connect(dsk, SIGNAL(setKernel(double*,int)),this,SLOT(applySetKernel(double*,int)));
        updateImageReference(this->image);
    }
    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));
}

void MainWindow::applyUndo()
{
    if (flag){
        updateImageReference(copy->copy());
    }

}

void MainWindow::applyBlurCustomDialog(){
    dialogCustom *dc=new dialogCustom();
    connect(dc, SIGNAL(custom(int,bool,bool,bool)),this, SLOT(applyBlurCustom(int,bool,bool,bool)));
    dc->show();
}

void MainWindow::dialogFusion(void){
    dialogfusion *df=new dialogfusion();
    connect(df, SIGNAL(fusion(QString,float ,int , int )),this, SLOT(applyFusion(QString,float ,int , int)));
    df->show();

}

void MainWindow::applyCrop(int startx,int starty,int endx,int endy){
    updateImageReference(image->ApplyCrop(startx,starty,endx,endy));
}
void MainWindow::applyMeanFilter(){
    image->makeMeanFilter(3);
    updateImageReference(this->image);
}

void MainWindow::applyIntelligentResize(){
    DialogScale *ds=new DialogScale(new QString("Inteligent Resizing"),this);
    connect(ds,SIGNAL(ScaleFired(float,float)),this,SLOT(applyIntelligentResize(float,float)));
    ds->show();

}

void MainWindow::applyIntelligentResize(float width,float height){

    dp=new DialogProgress(this);
    dp->setWindowFlags(Qt::FramelessWindowHint);
    dp->show();


    threadresize *operation=new threadresize(image,width,height);
    threadprogress *progress=new threadprogress(dp->getProgress());

    connect(operation,SIGNAL(finished(ImageAbstraction*)),this,SLOT(updateImageReference(ImageAbstraction*)));
    connect(operation,SIGNAL(finished(ImageAbstraction*)),dp,SLOT(close()));
    connect(operation,SIGNAL(finished(ImageAbstraction*)),progress,SLOT(terminate()));
    connect(progress,SIGNAL(progresschanged(int)),dp,SLOT(setProgress(int)));

    operation->start(QThread::NormalPriority);
    progress->start(QThread::NormalPriority);

}


void MainWindow::applyScale(){
    DialogScale *ds=new DialogScale(this);
    connect(ds,SIGNAL(ScaleFired(float,float)),this,SLOT(applyScale(float,float)));
    ds->show();
}

void MainWindow::applyScale(float width,float height){
    updateImageReference(image->ApplyScale(width,height));
}

void MainWindow::applyLaplacianFilter(){
    image->makeLaplacianFilter(5);
    updateImageReference(this->image);
}
void MainWindow::applyGradFilterX(){
    image->makeGradFilterX(3);
    updateImageReference(this->image);
}
void MainWindow::applyGradFilterY(){
    image->makeGradFilterY(3);
    updateImageReference(this->image);
}
