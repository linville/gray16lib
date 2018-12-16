/*!
  \file false_color_convert.cpp
  \brief Implementation of the FalseColorConvert class.
*/

#include "false_color_convert.h"

#include <QtDebug>
#include <QImage>

#include <algorithm>
#include <cmath>

using namespace Gray16;

void FalseColorConvert::setFalseColorMode(FalseColorMode falseColorMode) {
  mFalseColorMode = falseColorMode;
}

QImage FalseColorConvert::mConvert(const QImage &originalImage) const {
  switch(mFalseColorMode) {
  case FalseColorMode::BlackBody:
    return mBlackBodyConvert(originalImage);
  }

  return {};
}

/*!
  \brief Maps a 16-bit grayscale to a Black Body style gradient.

  16-bit values 0 to 0x5555 mapped as Black to Red
  16-bit values 0x5555 to 0xAAAA mapped as Red to White
  16-bit values 0xAAAA mapped White to Blue
*/

QImage FalseColorConvert::mBlackBodyConvert(const QImage &originalImage) const {
  QImage image(originalImage.width(), originalImage.height(), QImage::Format_RGB32);

  uint32_t *d = reinterpret_cast<uint32_t *> (image.bits());
  const uint16_t *src = reinterpret_cast<const uint16_t *> (originalImage.constBits());

  constexpr double scale = 0x5555 / 255.;

  const auto totalPixels = originalImage.width() * originalImage.height();
  for(int i = 0; i < totalPixels; ++i) {
    if(src[i] < 0x5555) {
      // Black to Red
      const auto tmp = static_cast<uint32_t> (std::round(src[i] / scale));
      d[i] = 0xFF000000 | (tmp << 16);
    } else if(src[i] >= 0x5555 && src[i] < 0xAAAA) {
      // Red to White
      const auto tmp = static_cast<uint32_t> (std::round((src[i] - 0x5555) / scale));
      d[i] = 0xFFFF0000 | (tmp << 8) | tmp;
    } else if(src[i] >= 0xAAAA) {
      // White to Blue
      const auto tmp = static_cast<uint32_t> (std::round((src[i] - 0xAAAA) / scale));
      const auto invTmp = 0xFF - tmp;
      d[i] = 0xFF0000FF | (invTmp << 16) | (invTmp << 8);
    }
  }

  return image;
}
