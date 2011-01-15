#include "dialoglog.h"
#include "ui_dialoglog.h"

dialogLoG::dialogLoG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogLoG)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));
}
void dialogLoG::okpressed(void){

    emit log(ui->spinBox->text().toInt(),ui->doubleSpinBox->text().toDouble());
}
dialogLoG::~dialogLoG()
{
    delete ui;
}
