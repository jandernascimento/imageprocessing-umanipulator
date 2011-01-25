#include "imageabstration.h"

#include <QThread>
#ifndef MYTHREAD_H
#define MYTHREAD_H

class threadresize : public QThread
{
    Q_OBJECT
public:
    explicit threadresize(ImageAbstraction *ia, QObject * apParent = 0);

protected:
    void run();

private:
    ImageAbstraction *ia;

signals:
    void finished(ImageAbstraction *ia);
};

#endif // MYTHREAD_H
