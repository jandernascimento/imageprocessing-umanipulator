#include "dialogscale.h"
#include "ui_dialogscale.h"

DialogScale::DialogScale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScale)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
}

DialogScale::DialogScale(QString *title,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScale)
{
    ui->setupUi(this);
    setWindowTitle(*title);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
}

DialogScale::~DialogScale()
{
    delete ui;
}

void DialogScale::okpressed(void){

    emit ScaleFired(((float)ui->spinBox->value())/100,((float)ui->spinBox_2->value())/100);

}

