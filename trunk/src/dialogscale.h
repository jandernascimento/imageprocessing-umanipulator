#ifndef DIALOGSCALE_H
#define DIALOGSCALE_H

#include <QDialog>

namespace Ui {
    class DialogScale;
}

class DialogScale : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScale(QWidget *parent = 0);
    explicit DialogScale(QString *title,QWidget *parent=0) ;
    ~DialogScale();

public slots:
    void okpressed(void);

signals:
    void ScaleFired(float width,float height);

private:
    Ui::DialogScale *ui;
};

#endif // DIALOGSCALE_H
