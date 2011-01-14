#ifndef DIALOGSETKERNEL_H
#define DIALOGSETKERNEL_H

#include <QDialog>

namespace Ui {
    class dialogSetKernel;
}

class dialogSetKernel : public QDialog
{
    Q_OBJECT

public:
    explicit dialogSetKernel(QWidget *parent = 0);
    ~dialogSetKernel();

private:
    Ui::dialogSetKernel *ui;
};

#endif // DIALOGSETKERNEL_H
