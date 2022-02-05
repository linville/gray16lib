/*!
  \file tracking_label.h
  \brief Specialized QLabel that enables mouse tracking.
*/

#ifndef TRACKING_LABEL_H
#define TRACKING_LABEL_H

#include <QtDebug>
#include <QtCore>
#include <QtWidgets>

/*!
  \brief Cursor highlighting label that emits coordinates in image coordinates.
*/

class TrackingLabel : public QLabel {
  Q_OBJECT

public:
  explicit TrackingLabel(QWidget *parent) : QLabel(parent) {
    setMouseTracking(true);
  }

  void setOtherLabel(TrackingLabel *otherLabel) {
    Q_ASSERT(otherLabel != nullptr);
    mOtherLabel = otherLabel;
  }

  void clearHighlighting() {
    mHighlightCoordinate = false;
    update();
  }

  void highlightCoordinate(int x, int y) {
    mHighlightCoordinate = true;
    mCursorX = x;
    mCursorY = y;

    update();
  }

signals:
  void imageCoordinate(int x, int y);

protected:
  void leaveEvent(QEvent *event) {
    Q_UNUSED(event)

    if(mOtherLabel) {
      mOtherLabel->clearHighlighting();
    }
  }

  void mouseMoveEvent(QMouseEvent *event) {
    if(pixmap().isNull()) {
      return;
    }

    const auto mousePoint = event->position().toPoint();
    highlightCoordinate(mousePoint.x(), mousePoint.y());

    if(mOtherLabel) {
      mOtherLabel->highlightCoordinate(mousePoint.x(), mousePoint.y());
    }

    int x = static_cast<int> ((static_cast<double> (pixmap().width()) / this->width())
                              * mousePoint.x());
    int y = static_cast<int> ((static_cast<double> (pixmap().height()) / this->height())
                              * mousePoint.y());

    emit imageCoordinate(x, y);
  }

  void paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    if(!mHighlightCoordinate) {
      return;
    }

    QPainter painter(this);

    painter.setPen(QPen(QBrush(QColor(255, 0, 0, 200)), 0));
    painter.drawEllipse(QPoint(mCursorX, mCursorY),
                        10, 10);
  }

private:
  QPointer<TrackingLabel> mOtherLabel;

  bool mHighlightCoordinate = false;
  int mCursorX = 0;
  int mCursorY = 0;
};

#endif // TRACKING_LABEL_H
