#include <QImage>
#ifndef IMAGEABSTRATION_H
#define IMAGEABSTRATION_H

class ImageAbstraction : public QImage
{
    public:
      ImageAbstraction(QWidget* parent=0);
      ImageAbstraction();
      ImageAbstraction(const QSize &size, Format format);
      ImageAbstraction(int width, int height, Format format);
      ImageAbstraction(uchar *data, int width, int height, Format format);
      ImageAbstraction(const uchar *data, int width, int height, Format format);
      ImageAbstraction(uchar *data, int width, int height, int bytesPerLine, Format format);
      ImageAbstraction(const uchar *data, int width, int height, int bytesPerLine, Format format);

      explicit ImageAbstraction(const QString &fileName, const char *format = 0);


};

#endif // IMAGEABSTRATION_H
