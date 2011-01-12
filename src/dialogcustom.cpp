#include "dialogcustom.h"
#include "ui_dialogcustom.h"

dialogCustom::dialogCustom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogCustom)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
}
void dialogCustom::okpressed(void){

    emit blur(ui->spinBox->text().toInt());
}
dialogCustom::~dialogCustom()
{
    delete ui;
}
