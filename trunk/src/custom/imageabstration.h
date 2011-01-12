#include <QImage>
#ifndef IMAGEABSTRATION_H
#define IMAGEABSTRATION_H

class ImageAbstraction : public QImage
{
     public:
          enum ecolor {red, green, blue, black, cyan, magenta, yellow};
          explicit ImageAbstraction(const QString &fileName, const char *format = 0);
          explicit ImageAbstraction(const QSize &size, Format format);
          /** Pixels get/set **/
          QRgb* getPixel(int x, int y);
          int getPixelColorIntensity(enum ecolor color,int x, int y);
          QRgb* setPixel(enum ecolor color, int x, int y,int value);
          QRgb* setPixel(int x, int y,int red, int green, int blue);

          /** Util  methods **/
          void UpdateColorRange(void);
          int getMinColorValue(enum ecolor color);
          int getMaxColorValue(enum ecolor color);
          int getColorCounter(enum ecolor color,int level);

          /** Filter and masks **/
          void ApplyFilterGreyScale();
          void ApplyFilterContrast(int min,int max);
          void ApplyFilterFusion(ImageAbstraction *fimage,float percentage,int posx,int posy);
          double* makeFilterGaussian(int dim, int sig);
          void ApplyConvolution(int dim, int sig, char filter);
          int RGB2CMYK(int x, int y, enum ecolor color);
          ImageAbstraction* ApplyCrop(int startx,int starty,int endx,int endy);


    private:
          int redmax;
          int redmin;
          int greenmax;
          int greenmin;
          int bluemax;
          int bluemin;
          int colorcounterred[256];
          int colorcountergreen[256];
          int colorcounterblue[256];
          int ApplyFilterContrastRule(enum ecolor color,int x,int y,int min,int max);

};

#endif // IMAGEABSTRATION_H
