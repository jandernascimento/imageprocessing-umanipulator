#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
     void quit(void);
private:
    Ui::MainWindow *ui;
    QMenu *filemenu;
    QMenu *viewmenu;
    QMenu *imagemenu;
    QMenu *colormenu;
    void setupFileMenu(QMenu *menu);
    void setupViewMenu(QMenu *menu);
    void setupImageMenu(QMenu *menu);
    void setupImageFilterSubMenu(QMenu *menu);
    void setupImageAdvancedSubMenu(QMenu *menu);
    void setupColorMenu(QMenu *menu);
    void configureQuit(QAction *act);

};

#endif // MAINWINDOW_H
