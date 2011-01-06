#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include "imagewindow.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    createMenu();

}

void MainWindow::createMenu(){

    //file
    filemenu=menuBar()->addMenu("&File");
    setupFileMenu(filemenu);

    //view
    viewmenu=menuBar()->addMenu("&View");
    setupViewMenu(viewmenu);

    //image
    imagemenu=menuBar()->addMenu("&Image");
    setupImageMenu(imagemenu);

    //image/filter
    QMenu *imagesubfilter=imagemenu->addMenu("&Filter");
    setupImageFilterSubMenu(imagesubfilter);

    //image/advanced
    imagemenu->addSeparator();
    QMenu *imagesubadvanced=imagemenu->addMenu("&Advanced");
    setupImageAdvancedSubMenu(imagesubadvanced);

    //color
    colormenu=menuBar()->addMenu("&Color");
    setupColorMenu(colormenu);
}

void MainWindow::setupFileMenu(QMenu *menu){
    QAction *openaction=new QAction(("&Open..."), this);
    QAction *saveaction=new QAction(("&Save"), this);
    QAction *saveasaction=new QAction(("Save &as..."), this);
    QAction *quitaction=new QAction(("&Quit"), this);

    configureOpen(openaction);
    configureSaveAs(saveasaction);
    configureQuit(quitaction);

    menu->addAction(openaction);
    menu->addAction(saveaction);
    menu->addAction(saveasaction);
    menu->addSeparator();
    menu->addAction(quitaction);
}

void MainWindow::setupViewMenu(QMenu *menu){

    QAction *colorbaraction=new QAction(("&Color information bar"), this);
    QAction *histogramaction=new QAction(("&Histogram"), this);

    menu->addAction(colorbaraction);
    menu->addAction(histogramaction);

}

void MainWindow::setupImageMenu(QMenu *menu){
    QAction *crop=new QAction(("&Crop"), this);
    QAction *blur=new QAction(("&Blur"), this);
    QAction *fusion=new QAction(("&Fusion"), this);
    QAction *resize=new QAction(("&Resize"), this);

    menu->addAction(crop);
    menu->addAction(blur);
    menu->addAction(fusion);
    menu->addAction(resize);
}

void MainWindow::setupImageFilterSubMenu(QMenu *menu){
    QAction *mean=new QAction(("Mean"), this);
    QAction *gradient=new QAction(("Gradiant"), this);
    QAction *gaussian=new QAction(("Gaussian"), this);
    QAction *laplacian=new QAction(("Laplacian"), this);
    QAction *custom=new QAction(("Custom"), this);

    menu->addAction(mean);
    menu->addAction(gradient);
    menu->addAction(gaussian);
    menu->addAction(laplacian);
    menu->addSeparator();
    menu->addAction(custom);

}

void MainWindow::setupImageAdvancedSubMenu(QMenu *menu){
    QAction *resizing=new QAction(("Intelligent resizing"), this);
    QAction *scissor=new QAction(("Intelligent scissor"), this);

    menu->addAction(resizing);
    menu->addAction(scissor);
}

void MainWindow::setupColorMenu(QMenu *menu){
    QAction *contrast=new QAction(("&Contrast and equalization"), this);
    QAction *convertGrey=new QAction(("Convert to &grey scale"), this);
    menu->addAction(contrast);
    menu->addAction(convertGrey);
}

void MainWindow::configureQuit(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(quit()));
}

void MainWindow::configureOpen(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(open()));
}

void MainWindow::configureSaveAs(QAction *act){
    connect(act, SIGNAL(triggered()),this, SLOT(saveas()));
}

void MainWindow::quit(void){
    QCoreApplication::exit();
}

void MainWindow::open(void){
    /*/creates a QFileDialog without using the static function
    QFileDialog dialog(this);
    //only files with these extension will be shown in the QFileDialog
    dialog.setNameFilter(tr("Images (*.gif *.jpg *.pnm *.png)"));
    //presents the contents of the current directory as a list of file and directory names
    dialog.setViewMode(QFileDialog::List);
    //the user must select an existing file
    dialog.setFileMode(QFileDialog::ExistingFile);
    //a modal file dialog is created and shown. If the user clicked OK, the file they selected is put in fileName
    QStringList fileNames;
    if (dialog.exec()){
         QMessageBox::information(this, tr("title"), tr("File choosed"));
         fileNames = dialog.selectedFiles();
         QMessageBox::information(this, tr("title"), tr("File choosed"));
         ImageWindow *w2=new ImageWindow(this,fileNames.at(0));
         w2->show();
     }
    */


    // creates a QFileDialog using static function
    //if (maybeSave()) {
             fileSelected = QFileDialog::getOpenFileName(this,
                tr("Open Image"), "", tr("Image Files (*.gif *.jpg *.pnm *.png)"), 0, QFileDialog::DontUseNativeDialog);
             if (!fileSelected.isEmpty()){
                 QMessageBox::information(this, tr("title"), fileSelected);
                 imagewin=new ImageWindow(this,fileSelected);
                 imagewin->show();
             }
      //   }

}

void MainWindow::save(void){
    imagewin->save(fileSelected);
}

void MainWindow::saveas(void){
    QFileInfo file=QFileDialog::getSaveFileName(this, tr("Save Image"), "",
        tr("Images (*.gif *.jpg *.pnm *.png)"), 0, QFileDialog:: DontUseNativeDialog);

    if ( ! file.fileName().isEmpty()){


        fileSelected = file.absoluteFilePath();
        if (file.suffix()==NULL)
            fileSelected = file.absoluteFilePath() + ".jpg";

        save();

        QMessageBox::information(this, tr("title"), fileSelected);
    }

}

bool MainWindow::maybeSave(void){
     //if (textEdit->document()->isModified()) {
         QMessageBox::StandardButton ret;
         ret = QMessageBox::warning(this, tr("Application"),
                      tr("The document has been modified.\n"
                         "Do you want to save your changes?"),
                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         /*if (ret == QMessageBox::Save)
             return save();
         else if (ret == QMessageBox::Cancel)
             return false;*/
     //}
     //return true;
 }

MainWindow::~MainWindow()
{
    delete ui;
}
