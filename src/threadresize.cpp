#include <QThread>
#include <QObject>
#include <imageabstration.h>
#include "threadresize.h"
#include "dialogprogress.h"


threadresize :: threadresize(ImageAbstraction *ia, QObject *apParents):QThread(apParents){
    this->ia=ia;
}

void threadresize::run()
{

    sleep(10);
    ia->ApplyFilterGreyScale();

    emit finished(ia);

}
