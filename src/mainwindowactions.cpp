#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "histogram.h"
#include "dialogscale.h"
#include "dialogabout.h"
#include "dialogsetkernel.h"
#include "dialoglog.h"


void MainWindow::histogram(void){
    if (fileSelected == NULL)
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

void MainWindow::open(void){
    QFileInfo file = QFileDialog::getOpenFileName(this,
                tr("Open Image"), "", tr("Image Files (*.gif *.jpg *.pnm *.png)"), 0, QFileDialog::DontUseNativeDialog);

    fileSelected=file.fileName();
    filePath=file.absoluteFilePath();

    if (!fileSelected.isEmpty()){

        image=new ImageAbstraction(filePath);
        label->setPixmap(QPixmap::fromImage(*image,Qt::AutoColor));

        connect(label,SIGNAL(selected(QMouseEvent*)),this,SLOT(mouseOver(QMouseEvent*)));
    }
}

void MainWindow::save(void){
    if (fileSelected == NULL)
        QMessageBox::warning(this, tr("Warning"), tr("There is no file in use."));
    else
        this->save(fileSelected);

}

void MainWindow::saveas(void){
    if (fileSelected == NULL)
        QMessageBox::warning(this, tr("Warning"), tr("There is no file in use."));
    else{
        QFileInfo file=QFileDialog::getSaveFileName(this, tr("Save Image"), "",
            tr("Images (*.gif *.jpg *.pnm *.png)"), 0, QFileDialog:: DontUseNativeDialog);

        if ( ! file.fileName().isEmpty()){
            fileSelected = file.absoluteFilePath();
            if (file.suffix()==NULL)
                fileSelected = file.absoluteFilePath() + ".jpg";

            save();
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
void MainWindow::applySetKernel(QString str){
    if (str.toDouble()==0)
        qDebug("NULLLLLLL");
    //qDebug("%f VALUE OF (1,1)",d);

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
void MainWindow::applySag(int dim)
{
    qDebug("In appply asasasndfdsnj");
    image->makeMeanFilter(dim);
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
        connect(dsk, SIGNAL(setKernel(QString)),this,SLOT(applySetKernel(QString)));
        //dsk->setSize(3);
        dsk->show();
        /*if (dim == 3)
        {
            dialogkernel3 *dk3 = new dialogkernel3();
            connect(dk3, SIGNAL(kernel3(double,double,double,double,double,double,double,double,double)),this,SLOT(applyCustomKernel3(double,double,double,double,double,double,double,double,double)));
            dk3->show();

        }
        else if (dim==4)
        {

                dialogkernel4 *dk4 = new dialogkernel4();

                connect(dk4, SIGNAL(kernel4(int)),this,SLOT(applySag(int)));
                dk4->show();



        }*/
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
