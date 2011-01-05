#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>

namespace Ui {
    class ImageWindow;
}

class ImageWindow : public QMainWindow
{
    Q_OBJECT;

public:
    explicit ImageWindow(QWidget *parent = 0);
    explicit ImageWindow(QWidget *parent = 0, QString filesname=NULL);
    ~ImageWindow();

public slots:
    void save(QString fileName);

private:
    Ui::ImageWindow *ui;
};

#endif // IMAGEWINDOW_H
