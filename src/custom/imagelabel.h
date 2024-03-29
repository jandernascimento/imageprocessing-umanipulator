#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include <QRubberBand>

class ImageLabel : public QLabel
{
    Q_OBJECT
    public:
    explicit ImageLabel(QWidget* parent=0);
    void setRubberband(bool status);

    signals:
      void selected(QMouseEvent* event);
      void areaselected(int startx,int starty, int endx, int endy);
    protected:
      void mousePressEvent( QMouseEvent* event );
      void mouseMoveEvent(QMouseEvent *e);
      void mouseReleaseEvent ( QMouseEvent * event );
      QRubberBand *rubberBand;
      bool rubberbandactivated;
      QPoint start;
      QPoint end;

};

#endif // IMAGELABEL_H
