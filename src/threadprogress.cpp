#include "threadprogress.h"
#include "dialogprogress.h"

threadprogress::threadprogress(QProgressBar *bar,QObject *parent) :
    QThread(parent)
{
    this->progressBar=bar;
}

void threadprogress::run(void){
    int x=0;

    emit progresschanged(0);

    for(;x<100;x++){

        usleep(10000);

        //qDebug("%i",x);
        if(x==99) x=0;

        emit progresschanged(x);

    }

}
