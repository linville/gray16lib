/*!
  \file manual_convert.cpp
  \brief Implementation of the ManualConvert class.
*/

#include "manual_convert.h"

#include <QtDebug>
#include <QImage>

#include <algorithm>
#include <cmath>

using namespace Gray16;

void ManualConvert::setExposurePoints(uint16_t blackPoint, uint16_t whitePoint) {
  Q_ASSERT(blackPoint <= MaxBlackPoint);
  Q_ASSERT(whitePoint >= MinWhitePoint);

  if(whitePoint - 255 < blackPoint) {
    qCritical() << "Invalid exposure settings: whitePoint - 255 < blackPoint." << endl
                << " blackPoint" << blackPoint << endl
                << " whitePoint" << whitePoint;
    return;
  }

  mBlackPoint = blackPoint;
  mWhitePoint = whitePoint;
}

QImage ManualConvert::mConvert(const QImage &originalImage) const {
  QImage image(originalImage.width(), originalImage.height(), QImage::Format_Grayscale8);

  uint8_t *d = reinterpret_cast<uint8_t *> (image.bits());
  const uint16_t *src = reinterpret_cast<const uint16_t *> (originalImage.constBits());

  const auto scale = (mWhitePoint - mBlackPoint) / 255.;

  const auto totalPixels = originalImage.width() * originalImage.height();
  for(int i = 0; i < totalPixels; ++i) {
    const int value = std::lrint((src[i] - mBlackPoint) / scale);
    d[i] = std::clamp(value, 0, 255);
  }

  return image;
}
