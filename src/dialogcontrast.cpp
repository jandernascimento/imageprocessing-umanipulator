#include "dialogcontrast.h"
#include "ui_dialogcontrast.h"
#include "QtCore/QtCore"
#include <QtGui>
#include <QPixmap>

DialogContrast::DialogContrast(QDialog *parent)
{
    this->setupUi(this); // this sets up GUI

    connect(horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(contrastChanged(int)));
    connect(horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(contrastChanged(int)));
}


void DialogContrast::contrastChanged(int value)
{
   label->setText(QString("Minimum (%1)").arg(horizontalSlider->value()));

   label_2->setText(QString("Maximum (%1)").arg(horizontalSlider_2->value()));

    /* QMessageBox::about(this,"About myQtApp",
                "This app was coded for educational purposes.\n"
                "Number 1 is: " + QString::number(spinBox1->value()) + "\n\n"
                "Bye.\n"); */

    if(horizontalSlider->value()>horizontalSlider_2->value()){

        int pad=0;

        if((horizontalSlider->value()+25)>255){
           pad=(horizontalSlider->value()+25)-255;
        }

        horizontalSlider->setValue(horizontalSlider->value()-pad);
        horizontalSlider_2->setValue(horizontalSlider->value()+25);
    }

    emit constrastChanged(this->horizontalSlider->value(),this->horizontalSlider_2->value());

}

void DialogContrast::setMinMax(int min,int max){

    horizontalSlider->setValue(min);
    horizontalSlider_2->setValue(max);

}



