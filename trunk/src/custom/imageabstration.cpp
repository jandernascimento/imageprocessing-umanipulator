#include "imageabstration.h"

ImageAbstraction::ImageAbstraction(const QString &fileName, const char *format):QImage(fileName, format ){
    qDebug("Calling custom constructor");
    for(int x=0;x<this->height();x++){
        for(int y=0;y<this->width();y++){
            //Sets all red values to the maximum, so the image becomes i little redy
            setPixel(ImageAbstraction::red,x,y,255);
        }
    }
}

QRgb* ImageAbstraction::getPixel(enum ecolor color, int x, int y){
    QRgb *pixel = (QRgb *)this->scanLine(x);
    pixel=(pixel+y);
    return pixel;
}

QRgb* ImageAbstraction::setPixel(enum ecolor color, int x, int y, int value){
    QRgb *pixel = getPixel(color,x,y);
    int red = color==ImageAbstraction::red?value:qRed(*pixel);
    int green = color==ImageAbstraction::green?value:qGreen(*pixel);
    int blue = color==ImageAbstraction::blue?value:qBlue(*pixel);
    *pixel = qRgba(red,green,blue,255);
    return pixel;

}

QRgb* ImageAbstraction::setPixel(int x, int y,int red, int green, int blue){
    setPixel(ImageAbstraction::red,x,y,red);
    setPixel(ImageAbstraction::green,x,y,green);
    return setPixel(ImageAbstraction::blue,x,y,blue);
}
