#include "dialogsetkernel.h"
#include "ui_dialogsetkernel.h"
#include "qmessagebox.h"

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
    bool ok;
    bool flag = true;
    for (int i=0;i<thisDim;++i)
        for (int j=0;j<thisDim;++j)
        {
                ui->tableWidget->item(i,j)->text().toDouble(&ok);
                if (ok)
                    vals[i*thisDim+j] = ui->tableWidget->item(i,j)->text().toDouble(&ok);
                else
                {
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem("ERROR"));
                    flag = false;
                }
        }
    if (flag)
        emit setKernel(vals,thisDim);
    else
    {
        flag = true;
        QMessageBox::warning(this, tr("Error"), tr("Kernel values must be doubles. Check the error cells please."));
        this->setVisible(false);
        this->show();
    }

}
void dialogSetKernel::setSize(int dim)
{
    thisDim = dim;
    for (int i=0; i<dim; ++i)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->insertColumn(i);
    }
    for (int i=0; i<dim; ++i)
        for (int j=0; j<dim; ++j)
            ui->tableWidget->setItem(i,j,new QTableWidgetItem("0.0"));

}

dialogSetKernel::~dialogSetKernel()
{
    delete ui;
}
