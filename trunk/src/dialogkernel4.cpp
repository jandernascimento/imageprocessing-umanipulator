#include "dialogkernel4.h"
#include "ui_dialogkernel4.h"

dialogkernel4::dialogkernel4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogkernel4)
{
    ui->setupUi(this);
    QWidget *sp[10];
    for (int i=0;i<10;++i)
        sp[i] = new QWidget();
    //for (int i=0;i<10;++i)
      //  sp[i]->show();
    //sp[0] = this->childAt(0,0);
    //sp[0]->




}

dialogkernel4::~dialogkernel4()
{
    delete ui;
}
