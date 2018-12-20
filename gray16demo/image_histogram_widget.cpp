/*!
  \file image_histogram_widget.cpp
  \brief Implementation of the ImageHistogramWidget class.
*/

#include "image_histogram_widget.h"

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

QVector<uint16_t> ImageHistogramWidget::calculateHistogram(const QImage &image, int bins) {
  Q_ASSERT(bins > 0);
  Q_ASSERT(bins <= 0xFFFF);

  if(bins == 0) {
    return {};
  }

  QVector<uint16_t> histogram(bins);

  auto *d = reinterpret_cast<const uint16_t *> (image.constBits());
  const int pixelCount = image.width() * image.height();

  const double bucketSize = 65536. / bins;

  for(int i = 0; i < pixelCount; ++i) {
    const auto bucket = static_cast<int> (std::floor(d[i] / bucketSize));
    histogram[bucket] += 1;
  }

  return histogram;
}

void ImageHistogramWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  const auto histogram = calculateHistogram(mImage, width());
  Q_ASSERT(histogram.size() == width());

  QPainter painter(this);

  painter.fillRect(geometry(), Qt::white);

  const auto y1 = height();
  const auto yScale = 65535. / height();
  qDebug() << y1 << yScale;

  painter.setPen(QPen(Qt::black, 0));
  for(int i = 0; i < width(); ++i) {
    qDebug() << histogram[i] << y1 << y1 - (histogram[i] / yScale);
    painter.drawLine(i, y1, i, y1 - (histogram[i] / yScale));
    //qDebug() << i << y1 << y1 - histogram[i];
  }
}
