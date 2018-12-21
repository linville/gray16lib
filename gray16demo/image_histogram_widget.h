/*!
  \file image_histogram_widget.h
  \brief Definition of the ImageHistogramWidget class.
*/

#ifndef IMAGE_HISTOGRAM_WIDGET_H
#define IMAGE_HISTOGRAM_WIDGET_H

#include <QtCore>
#include <QtWidgets>

class ImageHistogramWidget : public QWidget {
  Q_OBJECT

public:
  explicit ImageHistogramWidget(QWidget *parent);

  void setImage(const QImage &image);

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  static QVector<int> calculateHistogram(const QImage &image, int bins, int &outMaxValue);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QImage mImage; //!< QImage to generate histogram from.
};

#endif // IMAGE_HISTOGRAM_WIDGET_H

