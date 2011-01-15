#ifndef DIALOGLOG_H
#define DIALOGLOG_H

#include <QDialog>

namespace Ui {
    class dialogLoG;
}

class dialogLoG : public QDialog
{
    Q_OBJECT

public:
    explicit dialogLoG(QWidget *parent = 0);
    ~dialogLoG();
public slots:
    void okpressed(void);
signals:
    void log(int dim, double sig);

private:
    Ui::dialogLoG *ui;
};

#endif // DIALOGLOG_H
