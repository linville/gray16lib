/*!
  \file offset_gain_convert.cpp
  \brief Implementation of the OffsetGainConvert class.
*/

#include "offset_gain_convert.h"

#include <QtDebug>
#include <QImage>

#include <algorithm>

using namespace Gray16;

void OffsetGainConvert::setOffsetGain(int offset, int gain) {
  mOffset = offset;
  mGain = gain;
}

void OffsetGainConvert::setOffsetMode(OffsetMode offsetMode) {
  mOffsetMode = offsetMode;
}

int OffsetGainConvert::detectedOffset() const {
  return mOffset;
}

QImage OffsetGainConvert::mConvert(const QImage &originalImage) const {
  QImage image(originalImage.width(), originalImage.height(), QImage::Format_Grayscale8);

  switch(mOffsetMode) {
  case OffsetMode::Manual:
    break;
  case OffsetMode::LeastExposedOffset:
    mOffset = - leastExposedValue(originalImage);
    break;
  case OffsetMode::MostExposedOffset:
    mOffset = - mostExposedValue(originalImage) + 255;
    break;
  }

  uint8_t *d = reinterpret_cast<uint8_t *> (image.bits());
  const uint16_t *src = reinterpret_cast<const uint16_t *> (originalImage.constBits());

  const auto totalPixels = originalImage.width() * originalImage.height();
  for(int i = 0; i < totalPixels; ++i) {
    int value = (src[i] + mOffset);

    if(mGain > 0) {
      value <<= mGain;
    } else {
      value >>= abs(mGain);
    }

    d[i] = std::clamp(value, 0, 255);
  }

  return image;
}
