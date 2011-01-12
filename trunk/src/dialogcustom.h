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
    void blur(int dim);

private:
    Ui::dialogCustom *ui;
};

#endif // DIALOGCUSTOM_H
