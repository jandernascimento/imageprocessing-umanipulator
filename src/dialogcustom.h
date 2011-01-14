#ifndef DIALOGCUSTOM_H
#define DIALOGCUSTOM_H

#include <QDialog>

namespace Ui {
    class dialogCustom;
}

class dialogCustom : public QDialog
{
    Q_OBJECT

public:
    explicit dialogCustom(QWidget *parent = 0);
    ~dialogCustom();
public slots:
    void okpressed(void);
signals:
    void custom(int dim, bool r1, bool r2, bool r3);

private:
    Ui::dialogCustom *ui;
};

#endif // DIALOGCUSTOM_H
