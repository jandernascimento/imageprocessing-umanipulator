#include "qtextension.h"
#include <QLabel>
#include <QMouseEvent>

ImageLabel::ImageLabel(QWidget *parent){
    //Label(parent);
}

void ImageLabel::mousePressEvent (QMouseEvent* eve)
{
    if(eve->button() == Qt::LeftButton)
    {
        emit selected(eve);
    }
}

void ImageLabel::mouseMoveEvent( QMouseEvent* event ){
        //emit selected();
}

