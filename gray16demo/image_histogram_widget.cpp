/*!
  \file image_histogram_widget.cpp
  \brief Implementation of the ImageHistogramWidget class.
*/

#include "image_histogram_widget.h"

#include <algorithm>
#include <cmath>

ImageHistogramWidget::ImageHistogramWidget(QWidget *parent)
  : QWidget(parent) {

}

void ImageHistogramWidget::setImage(const QImage &image) {
  mImage = image;

  update();
}

QSize ImageHistogramWidget::minimumSizeHint() const {
  return QSize(400, 100);
}

QSize ImageHistogramWidget::sizeHint() const {
  return QSize(400, 200);
}

/*!
  \brief ImageHistogramWidget::calculateHistogram
  \param image QImage of which to generate histogram from.
  \param bins Number of histogram bins to calculate.
  \param outMaxValue Value of largest bin passed back out.
  \return QVector with an element for each histogram bin.

  \warning outMaxValue can be zero.
*/

QVector<int> ImageHistogramWidget::calculateHistogram(const QImage &image, int bins, int &outMaxValue) {
  Q_ASSERT(bins > 0);
  Q_ASSERT(bins <= 0xFFFF);

  outMaxValue = 0;

  if(image.isNull()) {
    return {};
  }

  Q_ASSERT(image.format() == QImage::Format_Grayscale16);

  if(bins == 0) {
    qCritical() << "Can't calculate histogram with 0 bins.";

    return {};
  }

  QVector<int> histogram(bins);

  auto *d = reinterpret_cast<const uint16_t *> (image.constBits());
  const int pixelCount = image.width() * image.height();

  const double bucketSize = 65536. / bins;

  for(int i = 0; i < pixelCount; ++i) {
    const auto bucket = static_cast<int> (std::floor(d[i] / bucketSize));
    histogram[bucket] += 1;
  }

  outMaxValue = *std::max_element(histogram.constBegin(), histogram.constEnd());

  return histogram;
}

void ImageHistogramWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  int maxValue = 0;
  const auto histogram = calculateHistogram(mImage, width(), maxValue);

  if(histogram.empty()
     || maxValue == 0) {
    return;
  }

  Q_ASSERT(histogram.size() == width());

  QPainter painter(this);
  painter.fillRect(geometry(), Qt::white);

  const auto y1 = height();
  const auto scale = static_cast<double> (height()) / static_cast<double> (maxValue);

  painter.setPen(QPen(Qt::black, 0));
  for(int i = 0; i < width(); ++i) {
    const int y2 = y1 - static_cast<int> (rint(histogram[i] * scale));

    painter.drawLine(i, y1, i, y2);
  }
}
