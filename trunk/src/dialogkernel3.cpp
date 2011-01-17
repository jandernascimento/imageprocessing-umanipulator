#include "dialogkernel3.h"
#include "ui_dialogkernel3.h"

dialogkernel3::dialogkernel3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogkernel3)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(okpressed()));



}
void dialogkernel3::okpressed()
{

    emit kernel3(ui->doubleSpinBox->text().toDouble(),
            ui->doubleSpinBox_2->text().toDouble(),
            ui->doubleSpinBox_3->text().toDouble(),
            ui->doubleSpinBox_4->text().toDouble(),
            ui->doubleSpinBox_5->text().toDouble(),
            ui->doubleSpinBox_6->text().toDouble(),
            ui->doubleSpinBox_7->text().toDouble(),
            ui->doubleSpinBox_8->text().toDouble(),
            ui->doubleSpinBox_9->text().toDouble()
            );
}

dialogkernel3::~dialogkernel3()
{
    delete ui;
}
