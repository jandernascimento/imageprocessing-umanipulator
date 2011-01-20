#ifndef DIALOGCONTRAST_H
#define DIALOGCONTRAST_H

#include "ui_dialogcontrast.h"
//#include "mainwindow.h"



class DialogContrast : public QDialog, private Ui::dialogcontrast
{
    Q_OBJECT

public:
    DialogContrast(QDialog *parent = 0);
    //void setWindow(MainWindow *pw);
    void setMinMax(int min,int max);

private:
    //MainWindow *win;
    int original_min;
    int original_max;

public slots:

    void contrastChanged(int value);
    void cancelpressed(void);

signals:
    void constrastChanged(int minimum,int maximum);

};


#endif

/*
class DialogContrast
{
public:
    DialogContrast();
};

#endif // DIALOGCONTRAST_H
*/
