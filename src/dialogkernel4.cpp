#include "dialogkernel4.h"
#include "ui_dialogkernel4.h"

dialogkernel4::dialogkernel4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogkernel4)
{
    ui->setupUi(this);
}

dialogkernel4::~dialogkernel4()
{
    delete ui;
}
