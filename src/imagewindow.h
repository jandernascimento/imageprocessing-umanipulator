#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>
#include <imagewindow.h>
#include <custom/imageabstration.h>

namespace Ui {
    class ImageWindow;
}

class ImageWindow : public QMainWindow
{
    Q_OBJECT;


public:
    explicit ImageWindow(QWidget *parent = 0);
    explicit ImageWindow(QWidget *parent = 0, QString filePath=NULL, QString fileName=NULL);
    ~ImageWindow();
    ImageAbstraction *image;

public slots:
    void save(QString fileName);
    void mouseOver(QMouseEvent* event);

private:
    Ui::ImageWindow *ui;

};

#endif // IMAGEWINDOW_H
