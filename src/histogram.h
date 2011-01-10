#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>

namespace Ui {
    class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = 0);
    ~Histogram();

private:
    Ui::Histogram *ui;
    void drawHistogram(void);
    void fillArray(int *n_colors,int n_items);
};

#endif // HISTOGRAM_H
