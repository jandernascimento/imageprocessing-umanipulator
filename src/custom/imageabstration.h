#include <QImage>
#ifndef IMAGEABSTRATION_H
#define IMAGEABSTRATION_H

class ImageAbstraction : public QImage
{

   public:
      enum ecolor {red, green, blue};
      explicit ImageAbstraction(const QString &fileName, const char *format = 0);
      QRgb* getPixel(enum ecolor color, int x, int y);
      QRgb* setPixel(enum ecolor color, int x, int y,int value);
      QRgb* setPixel(int x, int y,int red, int green, int blue);


};

#endif // IMAGEABSTRATION_H
