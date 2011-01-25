#include <QThread>
#include "custom/imageabstration.h"
#include "threadresize.h"


threadresize :: threadresize(ImageAbstraction *iax, float width, float height, QObject *apParents):QThread(apParents){
    this->image=iax;
    this->width=width;
    this->height=height;

}

void threadresize::run()
{
    emit finished(image->applySeamCarving(this->width,this->height));

}
