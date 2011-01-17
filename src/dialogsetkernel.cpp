#include "dialogsetkernel.h"
#include "ui_dialogsetkernel.h"
#include<QtCore>
#include<QValidator>
int thisDim;
dialogSetKernel::dialogSetKernel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSetKernel)
{
    ui->setupUi(this);
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
    thisDim = dim;
}

dialogSetKernel::~dialogSetKernel()
{
    delete ui;
}
