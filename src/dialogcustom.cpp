#include "dialogcustom.h"
#include "ui_dialogcustom.h"

dialogCustom::dialogCustom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogCustom)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
    //qDebug("%i",this->children().length());
}
void dialogCustom::okpressed(void){

    emit custom(ui->spinBox->text().toInt(),ui->radioButton->isChecked(),ui->radioButton_2->isChecked(),ui->radioButton_5->isChecked());
}
dialogCustom::~dialogCustom()
{
    delete ui;
}
