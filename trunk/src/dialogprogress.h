#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>
#include <QThread>
#include "QProgressBar"
namespace Ui {
    class DialogProgress;
}

class DialogProgress : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProgress(QWidget *parent = 0);

    QProgressBar* getProgress();
    ~DialogProgress();


private:
    Ui::DialogProgress *ui;

public slots:
    void setProgress(int value);
};

#endif // DIALOGPROGRESS_H
