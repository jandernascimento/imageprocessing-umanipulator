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
    void setSize(int dim);

public slots:
    void okpressed(void);
signals:
    void setKernel(QString d);

private:
    Ui::dialogSetKernel *ui;
};

#endif // DIALOGSETKERNEL_H
