#include "dialogkernel4.h"
#include "ui_dialogkernel4.h"

dialogkernel4::dialogkernel4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogkernel4)
{
    ui->setupUi(this);
    //QLayoutItem *sp = new QLayoutItem();
    //for (int i=0;i<10;++i)
      //  sp[i] = new QWidget();
    //ui->formLayout->addItem(sp);




}

dialogkernel4::~dialogkernel4()
{
    delete ui;
}
