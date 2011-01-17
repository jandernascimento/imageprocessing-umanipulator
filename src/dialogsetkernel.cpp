#include "dialogsetkernel.h"
#include "ui_dialogsetkernel.h"

dialogSetKernel::dialogSetKernel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSetKernel)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
    //qDebug("%i",ui->tableWidget->item(1,1)->text().toInt());
}
void dialogSetKernel::okpressed(void){

    emit setKernel(ui->tableWidget->item(0,0)->text());
}
void dialogSetKernel::setSize(int dim)
{
    //ui->tableWidget->itemAt(0,0)->flags().
}

dialogSetKernel::~dialogSetKernel()
{
    delete ui;
}
