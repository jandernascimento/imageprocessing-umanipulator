#include "dialogkernel4.h"
#include "ui_dialogkernel4.h"

dialogkernel4::dialogkernel4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogkernel4)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
}
void dialogkernel4::okpressed()
{
    int d = (int)ui->doubleSpinBox->text().toDouble();
        emit kernel4(d);
}
void dialogkernel4::setKernelSize(int dim)
{
    //QObjectList *list = new QObjectList();
        //if((bool)this->children().contains(QDoubleSpinBox))
            //qDebug("HA DARE!!!!");
        //list->at(0);
    this->childAt(10,10)->acceptDrops();

    //QDoubleSpinBox *spin = new QDoubleSpinBox();
    //qDebug("%i",this->y());
    //ui->doubleSpinBox->setParent(this);
    //QString str = this->children().at(1)->objectName();
    qDebug("%i",dim);
    //(QDoubleSpinBox*)obj;
    //obj->objectName()

}

dialogkernel4::~dialogkernel4()
{
    delete ui;
}
