#ifndef DIALOGFUSION_H
#define DIALOGFUSION_H

#include <QDialog>

namespace Ui {
    class dialogfusion;
}

class dialogfusion : public QDialog
{
    Q_OBJECT

signals:
    void fusion(QString path,float percentage,int x, int y);

public slots:
    void okpressed(void);
    void filechoose(void);

public:
    explicit dialogfusion(QWidget *parent = 0);
    ~dialogfusion();

private:
    Ui::dialogfusion *ui;
};

#endif // DIALOGFUSION_H
