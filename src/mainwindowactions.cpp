#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>


void MainWindow::histogram(void){
    QGraphicsScene scene;
    scene.addText("Hello, world!");

    QGraphicsView view(&scene);
    //view.centerOn(100,100);
    view.show();
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

