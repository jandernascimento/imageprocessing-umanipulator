#include "imagelabel.h"
#include <QRubberBand>


ImageLabel::ImageLabel(QWidget *parent){
    this->setParent(parent);
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberbandactivated=false;

}

void ImageLabel::mouseMoveEvent(QMouseEvent *e) {

    if(rubberbandactivated){
     rubberBand->setGeometry(QRect(start, e->pos()).normalized());
     rubberBand->show();
   }

}

void ImageLabel::mousePressEvent (QMouseEvent* event)
{

    if(rubberbandactivated){
        start=event->pos();

        rubberBand->setGeometry(QRect(start, start));
        rubberBand->show();

    }

    if(event->button() == Qt::LeftButton)
    {
        emit selected(event);
    }

}

void ImageLabel::mouseReleaseEvent ( QMouseEvent * event ){

    if(rubberbandactivated){
        end=event->pos();

        if((end.x()-start.x())>0&&
           (end.y()-start.y())>0){
            emit areaselected(start.x(),start.y(),end.x(),end.y());
        }

        rubberBand->hide();
    }

}

void ImageLabel::setRubberband(bool status){
    rubberbandactivated=status;
}
