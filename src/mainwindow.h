#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagewindow.h"
#include <custom/imagelabel.h>
#include <custom/imageabstration.h>
#include <mainwindow.h>
#include <dialogcontrast.h>
#include <dialogfusion.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void save(QString fileName);
    ImageAbstraction *image;
    ImageLabel *label;

public slots:
     void open(void);
     void saveas(void);
     void save(void);
     void quit(void);
     void dialogContrast(void);
     void dialogFusion(void);
     void mouseOver(QMouseEvent* event);
     void histogram(void);
     void applyContrast(int,int);
     void applyFusion(QString path, float percentage,int x, int y);
     void applyCrop(int startx,int starty,int endx,int endy);
     void applyGrey(void);

private:
    Ui::MainWindow *ui;
    ImageWindow *imagewin;
    DialogContrast mDialogContrast;
    QString fileSelected;
    QString filePath;
    QMenu *filemenu;
    QMenu *viewmenu;
    QMenu *imagemenu;
    QMenu *colormenu;
    void createMenu(void);
    void setupFileMenu(QMenu *menu);
    void setupViewMenu(QMenu *menu);
    void setupImageMenu(QMenu *menu);
    void setupImageFilterSubMenu(QMenu *menu);
    void setupImageAdvancedSubMenu(QMenu *menu);
    void setupColorMenu(QMenu *menu);
    void configureOpen(QAction *act);
    void configureSave(QAction *act);
    void configureContrast(QAction *act);
    void configureGrey(QAction *act);
    void configureFusion(QAction *act);
    void configureSaveAs(QAction *act);
    void configureQuit(QAction *act);
    void configureHistogram(QAction *act);
};

#endif // MAINWINDOW_H
