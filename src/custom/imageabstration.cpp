#include "imageabstration.h"

ImageAbstraction::ImageAbstraction(QWidget* parent){

}

ImageAbstraction::ImageAbstraction(){

    QImage();


};

ImageAbstraction::ImageAbstraction(const QSize &size, Format format){

    QImage(size, format);

};

ImageAbstraction::ImageAbstraction(int width, int height, Format format){

    QImage(width, height, format);


};

ImageAbstraction::ImageAbstraction(uchar *data, int width, int height, Format format){

    QImage(data, width, height, format);


};

ImageAbstraction::ImageAbstraction(const uchar *data, int width, int height, Format format){

    QImage(data, width, height, format);


};

ImageAbstraction::ImageAbstraction(uchar *data, int width, int height, int bytesPerLine, Format format){

    QImage(data, width, height, bytesPerLine, format);


};

ImageAbstraction::ImageAbstraction(const uchar *data, int width, int height, int bytesPerLine, Format format){

   QImage(data, width, height, bytesPerLine, format);

};

ImageAbstraction::ImageAbstraction(const QString &fileName, const char *format):QImage(fileName, format ){

    qDebug("Calling custom constructor");

}
