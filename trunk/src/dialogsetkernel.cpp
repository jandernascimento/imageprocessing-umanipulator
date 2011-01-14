#include "dialogsetkernel.h"
#include "ui_dialogsetkernel.h"

dialogSetKernel::dialogSetKernel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSetKernel)
{
    ui->setupUi(this);
}

dialogSetKernel::~dialogSetKernel()
{
    delete ui;
}
