#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagewindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void open(void);
     void saveas(void);
     void save(void);
     bool maybeSave(void);
     void quit(void);
private:
    Ui::MainWindow *ui;
    ImageWindow *imagewin;
    QString fileSelected;
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
    void configureSaveAs(QAction *act);
    void configureQuit(QAction *act);
};

#endif // MAINWINDOW_H
