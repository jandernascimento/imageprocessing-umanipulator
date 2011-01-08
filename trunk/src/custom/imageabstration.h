#include <QImage>
#ifndef IMAGEABSTRATION_H
#define IMAGEABSTRATION_H

class ImageAbstraction : public QImage
{
     public:
          enum ecolor {red, green, blue};
          explicit ImageAbstraction(const QString &fileName, const char *format = 0);
          /** Pixels get/set **/
          QRgb* getPixel(int x, int y);
          int getPixelColorIntensity(enum ecolor color,int x, int y);
          QRgb* setPixel(enum ecolor color, int x, int y,int value);
          QRgb* setPixel(int x, int y,int red, int green, int blue);

          /** Util  methods **/
          void UpdateColorRange(void);
          int getMinColorValue(enum ecolor color);
          int getMaxColorValue(enum ecolor color);

          /** Filter and masks **/
          void ApplyFilterGreyScale();
          void ApplyFilterContrast(int min,int max);

    private:
          int redmax;
          int redmin;
          int greenmax;
          int greenmin;
          int bluemax;
          int bluemin;

};

#endif // IMAGEABSTRATION_H
