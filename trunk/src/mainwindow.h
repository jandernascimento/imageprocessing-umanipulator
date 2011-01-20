#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagewindow.h"
#include <custom/imagelabel.h>
#include <custom/imageabstration.h>
#include <mainwindow.h>
#include <dialogcontrast.h>
#include <dialogcustom.h>
#include <dialogfusion.h>
#include <QFormLayout>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void scaleImage(double factor);
    ~MainWindow();
    ImageAbstraction *image;
    ImageLabel *label;

public slots:
     void open(void);
     void saveas(void);
     void save(void);
     void quit(void);
     void dialogContrast(void);
     void dialogAbout(void);
     void dialogFusion(void);
     void mouseOver(QMouseEvent* event);
     void histogram(void);
     void applyContrast(int,int);
     void applyFusion(QString path, float percentage,int x, int y);
     void applyCrop(int startx,int starty,int endx,int endy);
     void applyCrop();
     void applyScale();
     void applyScale(float width,float height);
     void applyGrey(void);
     void applyBlur(void);
     void applyBlurCustom(int dim, bool r1,bool r2,bool r3);
     void applyBlurCustomDialog(void);
     void applyMeanFilter();
     void applyLaplacianFilter();
     void applyGradFilterX();
     void applyGradFilterY();
     void applyCustomLoG();
     void applyLoG(int dim, double sig);
     void applySetKernel(double* vals, int dim);
     void applyTEMP();
     void findPaths(void);

private:
    Ui::MainWindow *ui;
    ImageWindow *imagewin;
    DialogContrast *mDialogContrast;
    dialogCustom mDialogCustom;
    QString filePath;
    QString fileName;
    QMenu *filemenu;
    QMenu *viewmenu;
    QMenu *imagemenu;
    QMenu *colormenu;
    QMenu *helpmenu;
    QMenu *teste;
    QToolBar *secondToolBar;
    void findPaths2(void);
    void highlightPaths(int * energy_matrix);
    int findMinValue(int value1,int value2, int value3);
    int findColumnMinValue(int * energy_matrix,int lin,int prev_col,int col,int next_col);
    int findMaxValue(int value1,int value2, int value3);
    int findColumnMaxValue(int * energy_matrix,int lin,int prev_col,int col,int next_col);
    void createEnergyMatrix(int * energy_matrix);
    void printMatrix(int * energy_matrix);
    void saveImage();
    void openFile(void);
    void createMenu(void);
    void setupFileMenu(QMenu *menu);
    void setupViewMenu(QMenu *menu);
    void setupImageMenu(QMenu *menu);
    void setupImageFilterSubMenu(QMenu *menu);
    void setupImageAdvancedSubMenu(QMenu *menu);
    void setupColorMenu(QMenu *menu);
    void setupHelpMenu(QMenu *menu);
    void configureOpen(QAction *act);
    void configureSave(QAction *act);
    void configureContrast(QAction *act);
    void configureGrey(QAction *act);
    void configureFusion(QAction *act);
    void configureSaveAs(QAction *act);
    void configureQuit(QAction *act);
    void configureHistogram(QAction *act);
    QAction* retrieveMenuOption(QString name, QMenu *menu);
};

#endif
