#ifndef DIALOGKERNEL_H
#define DIALOGKERNEL_H

#include <QDialog>

namespace Ui {
    class dialogKernel;
}

class dialogKernel : public QDialog
{
    Q_OBJECT

public:
    explicit dialogKernel(QWidget *parent = 0);
    ~dialogKernel();

private:
    Ui::dialogKernel *ui;
};

#endif // DIALOGKERNEL_H
