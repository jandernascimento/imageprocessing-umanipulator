#include "dialogcontrast.h"
#include "ui_dialogcontrast.h"
#include "QtCore/QtCore"
#include <QtGui>
#include <QPixmap>

DialogContrast::DialogContrast(QDialog *parent)
{
    this->setupUi(this); // this sets up GUI

    QString s=QString("%1").arg(this->label->text());

    qDebug(s.toAscii());

    connect(horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(contrastChanged(int)));
    connect(horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(contrastChanged(int)));


}


void DialogContrast::contrastChanged(int value)
{
   /* QMessageBox::about(this,"About myQtApp",
                "This app was coded for educational purposes.\n"
                "Number 1 is: " + QString::number(spinBox1->value()) + "\n\n"
                "Bye.\n");*/
    QString v=QString("%1").arg(value);

    qDebug(v.toAscii());

    emit constrastChanged(this->horizontalSlider->value(),this->horizontalSlider_2->value());

}



