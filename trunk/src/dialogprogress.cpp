#include "dialogprogress.h"
#include "ui_dialogprogress.h"
#include "QThread"

DialogProgress::DialogProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress)
{
    ui->setupUi(this);
}

QProgressBar* DialogProgress::getProgress(){
    return ui->progressBar;
}

void DialogProgress::setProgress(int value){

    ui->progressBar->setValue(value);


}

DialogProgress::~DialogProgress()
{
    delete ui;
}


