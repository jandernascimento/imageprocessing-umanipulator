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
    explicit ImageWindow(QWidget *parent = 0, QString fileName=NULL);
    void init(void);
    ~ImageWindow();

public slots:
    void save(QString fileName);
    void mouseOver(QMouseEvent* event);

private:
    Ui::ImageWindow *ui;
};

#endif // IMAGEWINDOW_H
