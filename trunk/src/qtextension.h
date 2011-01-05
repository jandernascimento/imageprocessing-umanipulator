#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

#ifndef QTEXTENSION_H
#define QTEXTENSION_H

class ImageLabel : public QLabel
{
    Q_OBJECT
    public:
    explicit ImageLabel(QWidget* parent=0);

    signals:
      void selected(QMouseEvent* event);
    protected:
      void mousePressEvent( QMouseEvent* event );
      void mouseMoveEvent( QMouseEvent* event );

};

#endif // QTEXTENSION_H
