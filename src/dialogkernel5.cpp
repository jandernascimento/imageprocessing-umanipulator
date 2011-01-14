#include "dialogkernel5.h"
#include "ui_dialogkernel5.h"

dialogkernel5::dialogkernel5(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogkernel5)
{
    ui->setupUi(this);
}

dialogkernel5::~dialogkernel5()
{
    delete ui;
}
