#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <custom/imageabstration.h>
#include <QGraphicsScene>

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
    void drawAllHistograms();
    void drawHistogram(enum ImageAbstraction::ecolor color,QGraphicsScene *scene);

private:
    Ui::Histogram *ui;    
    void fillArrayRealValues(enum ImageAbstraction::ecolor color,int *n_colors);
    void fillArrayCalculatedValues(enum ImageAbstraction::ecolor color,int *n_colors,int max_number_pixels,int limit_y);
};

#endif // HISTOGRAM_H
