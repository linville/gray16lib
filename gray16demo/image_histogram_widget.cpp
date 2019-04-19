/*!
  \file image_histogram_widget.cpp
  \brief Implementation of the ImageHistogramWidget class.
*/

#include "image_histogram_widget.h"

#include <algorithm>
#include <cmath>

ImageHistogramWidget::ImageHistogramWidget(QWidget *parent)
  : QWidget(parent) {

  QBrush cursorBrush(QColor(255, 0, 0, 200));
  mCursorPen.setBrush(cursorBrush);
  mCursorPen.setWidth(1);
}

void ImageHistogramWidget::setImage(const QImage &image) {
  mImage = image;
  mHistogram.clear();
  mHistogramPixmap = QPixmap();

  update();
}

void ImageHistogramWidget::highlightValue(int value) {
  mHighlightValue = value;
  update();
}

QSize ImageHistogramWidget::minimumSizeHint() const {
  return QSize(400, 100);
}

QSize ImageHistogramWidget::sizeHint() const {
  return QSize(400, 200);
}

/*!
  \brief Calculates the histogram data.
  \param image QImage of which to calculate a histogram from.
  \param bins Number of histogram bins to calculate.
  \param outMaxHistogramValue Max value of any of the bins.
  \return QVector with an element for each histogram bin.
*/

QVector<int> ImageHistogramWidget::calculateHistogram(const QImage &image, int bins, int &outMaxHistogramValue) {
  Q_ASSERT(bins > 0);
  Q_ASSERT(bins <= 0xFFFF);

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

  outMaxHistogramValue = *std::max_element(histogram.constBegin(), histogram.constEnd());

  return histogram;
}

/*!
  \brief Draws the histogram to a QPixmap
  \param height Height of desired QPixmap (width is calculated from Histogram vector size).
  \param maxHistogramValue Max value of histogram (-1 if unknown and it will be calculated)
  \return QPixmap of the QImage histogram.
*/

QPixmap ImageHistogramWidget::drawHistogram(const QVector<int> histogram, int height, int maxHistogramValue) {
  const int width = histogram.size();

  QPixmap pixmap(width, height);
  QPainter painter(&pixmap);

  if(maxHistogramValue < 0) {
    maxHistogramValue = *std::max_element(histogram.constBegin(), histogram.constEnd());
  }

  painter.fillRect(0, 0, width, height, Qt::gray);

  // If caculated histogram is null (usually an empty QImage)
  if(histogram.empty() || maxHistogramValue == 0) {
    return pixmap;
  }

  const auto scale = static_cast<double> (height) / static_cast<double> (maxHistogramValue);

  // Draw Histogram Values
  painter.setPen(QPen(Qt::black, 1));
  for(int i = 0; i < width; ++i) {
    const int &y1 = height;  // Read ability
    const int y2 = y1 - static_cast<int> (rint(histogram[i] * scale));

    painter.drawLine(i, y1,
                     i, y2);
  }

  // Draw Cumulative Value
  int sum = std::reduce(histogram.constBegin(), histogram.constEnd());
  const auto sumScale = static_cast<double> (height) / static_cast<double> (sum);

  painter.setPen(QPen(Qt::blue, 1));

  int accum = histogram[0];
  for(int i = 1; i < width; ++i) {
    const int y1 = height - static_cast<int> (rint(accum * sumScale));

    accum += histogram[i];
    const int y2 = height - static_cast<int> (rint(accum * sumScale));

    painter.drawLine(i - 1, y1,
                     i, y2);
  }

  return pixmap;
}

QPixmap ImageHistogramWidget::mGetCachedHistogram() {
  if(mHistogramPixmap.isNull()
     || width() != mHistogram.size()
     || mHistogramPixmap.width() != width()) {
    mHistogram = calculateHistogram(mImage, width(), mMaxHistogramValue);
    mHistogramPixmap = drawHistogram(mHistogram, height(), mMaxHistogramValue);
    return mHistogramPixmap;
  }

  if(mHistogramPixmap.height() != height()) {
    mHistogramPixmap = drawHistogram(mHistogram, height(), mMaxHistogramValue);
    return mHistogramPixmap;
  }

  return mHistogramPixmap;
}

void ImageHistogramWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.drawPixmap(0, 0, mGetCachedHistogram());

  // Draw Cursor Highlight
  painter.setPen(mCursorPen);
  if(mHighlightValue >= 0) {
    const double bucketSize = 65536. / width();
    const auto bucket = static_cast<int> (std::floor(mHighlightValue / bucketSize));

    const auto scale = static_cast<double> (height()) / static_cast<double> (mMaxHistogramValue);
    const int y1 = height() - static_cast<int> (rint(mHistogram[bucket] * scale));

    painter.drawLine(bucket, 0, bucket, height());

    painter.setBrush(mCursorPen.color());
    painter.drawEllipse(QPoint(bucket, y1), 3, 3);
  }
}

