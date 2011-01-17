#include "dialogsetkernel.h"
#include "ui_dialogsetkernel.h"

int thisDim;
dialogSetKernel::dialogSetKernel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSetKernel)
{
    ui->setupUi(this);
    //ui->tableWidget->itemAt(0,0)->setText("HAMID");
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));

}
void dialogSetKernel::okpressed(void)
{
    double* vals = (double*)(malloc(sizeof(double)*thisDim*thisDim));
    for (int i=0;i<thisDim;++i)
        for (int j=0;j<thisDim;++j)
            vals[i*thisDim+j] = ui->tableWidget->item(i,j)->text().toDouble();
    emit setKernel(vals,thisDim);

}
void dialogSetKernel::setSize(int dim)
{
    //qDebug("ROW %i",ui->tableWidget->rowCount());
    //qDebug("CLOUMN %i",ui->tableWidget->columnCount());
    //ui->tableWidget->windowFlags().
    thisDim = dim;
}

dialogSetKernel::~dialogSetKernel()
{
    delete ui;
}
