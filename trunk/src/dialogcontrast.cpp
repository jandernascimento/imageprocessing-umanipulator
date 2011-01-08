#include "dialogcontrast.h"
#include "ui_dialogcontrast.h"
#include "QtCore/QtCore"
#include <QtGui>
//#include <mainwindow.h>
#include <QPixmap>

DialogContrast::DialogContrast(QDialog *parent)
{
    this->setupUi(this); // this sets up GUI

    QString s=QString("%1").arg(this->label->text());

    qDebug(s.toAscii());

    connect(horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(contrastChanged(int)));


}


void DialogContrast::contrastChanged(int value)
{
   /* QMessageBox::about(this,"About myQtApp",
                "This app was coded for educational purposes.\n"
                "Number 1 is: " + QString::number(spinBox1->value()) + "\n\n"
                "Bye.\n");*/
    QString v=QString("%1").arg(value);

    qDebug(v.toAscii());

    emit constrastChanged(value,this->horizontalSlider_2->value());

    //this->win->image->ApplyFilterContrast(value);

    //this->win->label->setPixmap(QPixmap::fromImage(*this->win->image,Qt::AutoColor));

}

/*
void DialogContrast::setWindow(MainWindow *pw){
    this->win=pw;
}
*/


