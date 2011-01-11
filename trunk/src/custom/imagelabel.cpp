#include "imagelabel.h"
#include <QRubberBand>


ImageLabel::ImageLabel(QWidget *parent){
    this->setParent(parent);

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

}

void ImageLabel::mouseMoveEvent(QMouseEvent *e) {

     rubberBand->setGeometry(QRect(start, e->pos()).normalized());
     rubberBand->show();

}

void ImageLabel::mousePressEvent (QMouseEvent* event)
{

    start=event->pos();

    rubberBand->setGeometry(QRect(start, start));
    rubberBand->show();

    if(event->button() == Qt::LeftButton)
    {
        emit selected(event);
    }



}

void ImageLabel::mouseReleaseEvent ( QMouseEvent * event ){
    qDebug("Mouse released");
    end=event->pos();

    emit areaselected(start.x(),start.y(),end.x(),end.y());

}

/*
void ImageLabel::areaselected(int startx,int starty, int endx, int endy){



}
*/



