#ifndef DIALOGKERNEL4_H
#define DIALOGKERNEL4_H

#include <QDialog>

namespace Ui {
    class dialogkernel4;
}

class dialogkernel4 : public QDialog
{
    Q_OBJECT

public:
    explicit dialogkernel4(QWidget *parent = 0);
    ~dialogkernel4();
    void setKernelSize(int dim);

public slots:

    void okpressed(void);
signals:
    void kernel4(int d);
private:
    Ui::dialogkernel4 *ui;
};

#endif // DIALOGKERNEL4_H
