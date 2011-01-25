#ifndef THREADPROGRESS_H
#define THREADPROGRESS_H

#include <QThread>
#include <QProgressBar>
#include <dialogprogress.h>

class threadprogress : public QThread
{
    Q_OBJECT
public:
    explicit threadprogress(QProgressBar *bar,QObject *parent = 0);

protected:
    void run(void);
private:
    QProgressBar *progressBar;

signals:
    void progresschanged(int value);

public slots:

};

#endif // THREADPROGRESS_H
