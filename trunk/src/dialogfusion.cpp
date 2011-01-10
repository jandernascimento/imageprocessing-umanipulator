#include "dialogfusion.h"
#include "ui_dialogfusion.h"
#include <QFileInfo>
#include <QFileDialog>

dialogfusion::dialogfusion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogfusion)
{
    ui->setupUi(this);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(filechoose()));

}

dialogfusion::~dialogfusion()
{
    delete ui;
}

void dialogfusion::okpressed(void){

    emit fusion(ui->pathEdit->text(),ui->percentageEdit->text().toFloat()/100,ui->xEdit->text().toInt(),ui->yEdit->text().toInt());

}

void dialogfusion::filechoose(){

    QFileInfo fileSelected=QFileDialog::getOpenFileName(this,tr("Open Image"), "", tr("Image Files (*.gif *.jpg *.pnm *.png)"), 0, QFileDialog::DontUseNativeDialog);

    ui->pathEdit->setText(fileSelected.absoluteFilePath());


}
