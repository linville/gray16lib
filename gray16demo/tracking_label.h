/*!
  \file tracking_label.h
  \brief Specialized QLabel that enables mouse tracking.
*/

#ifndef TRACKING_LABEL_H
#define TRACKING_LABEL_H

#include <QtDebug>
#include <QLabel>
#include <QMouseEvent>

class TrackingLabel : public QLabel {
  Q_OBJECT

public:
  explicit TrackingLabel(QWidget *parent) : QLabel(parent) {
    setMouseTracking(true);
  }

signals:
  void imageCoordinate(int x, int y);

protected:
  void mouseMoveEvent(QMouseEvent *event) {
    if(pixmap()) {
      const auto mousePoint = event->localPos();

      int x = static_cast<int> ((static_cast<double> (pixmap()->width()) / this->width())
                                * mousePoint.x());
      int y = static_cast<int> ((static_cast<double> (pixmap()->height()) / this->height())
                                * mousePoint.y());

      emit imageCoordinate(x, y);
    }
  }
};

#endif // TRACKING_LABEL_H
