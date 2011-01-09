#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <dialogcontrast.h>
#include <QPixmap>
#include "histogram.h"

void MainWindow::histogram(void){
    Histogram *histowin=new Histogram(this);
    histowin->show();
}

void MainWindow::quit(void){
    QCoreApplication::exit();
}

void MainWindow::open(void){
    //creates a QFileDialog without using the static function
    //QFileDialog dialog(this);
    //only files with these extension will be shown in the QFileDialog
    //dialog.setNameFilter(tr("Images (*.gif *.jpg *.pnm *.png)"));
    //presents the contents of the current directory as a list of file and directory names
    //dialog.setViewMode(QFileDialog::List);
    //the user must select an existing file
    //dialog.setFileMode(QFileDialog::ExistingFile);
    //a modal file dialog is created and shown. If the user clicked OK, the file they selected is put in fileName
    //QStringList fileNames;
    //if (dialog.exec()){
      //   QMessageBox::information(this, tr("title"), tr("File choosed"));
        // fileNames = dialog.selectedFiles();
         //QMessageBox::information(this, tr("title"), tr("File choosed"));
         //ImageWindow *w2=new ImageWindow(this,fileNames.at(0));
         //w2->show();
     //}



    // creates a QFileDialog using static function
    QFileInfo file = fileSelected=QFileDialog::getOpenFileName(this,
                tr("Open Image"), "", tr("Image Files (*.gif *.jpg *.pnm *.png)"), 0, QFileDialog::DontUseNativeDialog);

    fileSelected=file.fileName();

    if (!fileSelected.isEmpty()){
        //imagewin=new ImageWindow(this,file.absoluteFilePath(),fileSelected);
        //imagewin->show();

        this->setWindowTitle(fileSelected);

        image=new ImageAbstraction(file.absoluteFilePath(),0);
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

   mDialogContrast.show();
   connect(&mDialogContrast,
           SIGNAL(constrastChanged(int,int)),
           this,
           SLOT(applyContrast(int,int)));

}

void MainWindow::applyContrast(int newmin,int newmax){

    qDebug("Chegou");

    image->ApplyFilterContrast(newmin,newmax);

    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));


}

void MainWindow::applyGrey(){

    image->ApplyFilterGreyScale();

    label->setPixmap(QPixmap::fromImage(*this->image,Qt::AutoColor));

}
