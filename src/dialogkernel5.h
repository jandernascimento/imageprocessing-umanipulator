#ifndef DIALOGKERNEL5_H
#define DIALOGKERNEL5_H

#include <QDialog>

namespace Ui {
    class dialogkernel5;
}

class dialogkernel5 : public QDialog
{
    Q_OBJECT

public:
    explicit dialogkernel5(QWidget *parent = 0);
    ~dialogkernel5();

private:
    Ui::dialogkernel5 *ui;
};

#endif // DIALOGKERNEL5_H
