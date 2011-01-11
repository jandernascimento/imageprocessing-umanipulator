#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include "custom/imageabstration.h"

namespace Ui {
    class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = 0);
    ~Histogram();
    ImageAbstraction *image;
    void drawHistogram(void);
private:
    Ui::Histogram *ui;    
    void fillArray(int *n_colors,int n_items);
};

#endif // HISTOGRAM_H
