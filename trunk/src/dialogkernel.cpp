#include "dialogkernel.h"
#include "ui_dialogkernel.h"

dialogKernel::dialogKernel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogKernel)
{
    ui->setupUi(this);
}

dialogKernel::~dialogKernel()
{
    delete ui;
}
