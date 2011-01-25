#include "imageabstration.h"
#include <QThread>
#ifndef THREADRESIZE_H
#define THREADRESIZE_H

class threadresize : public QThread
{
    Q_OBJECT
public:
    explicit threadresize(ImageAbstraction *ia, float width,float height, QObject * apParent = 0);

protected:
    void run();

private:
    ImageAbstraction *image;
    float width;
    float height;

signals:
    void finished(ImageAbstraction *image);
};

#endif // THREADRESIZE_H
