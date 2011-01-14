#ifndef DIALOGKERNEL3_H
#define DIALOGKERNEL3_H

#include <QDialog>

namespace Ui {
    class dialogkernel3;
}

class dialogkernel3 : public QDialog
{
    Q_OBJECT

public:
    explicit dialogkernel3(QWidget *parent = 0);
    ~dialogkernel3();
public slots:
    void okpressed(void);
signals:
    void kernel3(double d1,
                 double d2,
                 double d3,
                 double d4,
                 double d5,
                 double d6,
                 double d7,
                 double d8,
                 double d9
                 );
private:
    Ui::dialogkernel3 *ui;
};

#endif // DIALOGKERNEL3_H
