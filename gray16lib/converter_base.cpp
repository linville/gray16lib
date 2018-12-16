/*!
  \file converter_base.cpp
  \brief Implementation of the ConverterBase class.
*/

#include "converter_base.h"

#include <QtDebug>
#include <QImage>

using namespace Gray16;

/*!
  \brief Converts a Grayscale16 formatted QImage using the specialized behavior of the subclass.
  \param sourceImage Grayscale16 formatted QImage
*/

QImage ConverterBase::convert(const QImage &originalImage) const {
  if(originalImage.format() != QImage::Format_Grayscale16) {
    qCritical() << "Source image not Format_grayscale16:" << originalImage.format();

    return originalImage;
  }

  return mConvert(originalImage);
}


/*!
  \brief Finds the least-exposed pixel that isn't zero.

  Used with OffsetGainConvert to automatically set the offset.

  \sa OffsetGainConvert
*/

int ConverterBase::leastExposedValue(const QImage &image) {
  uint16_t autoBlackPoint = 0xFFFF;

  auto *d = reinterpret_cast<const uint16_t *> (image.constBits());
  auto count = image.width() * image.height();

  for(int i = 0; i < count; ++i) {
    if(d[i] > 0) {
      autoBlackPoint = std::min(d[i], autoBlackPoint);
    }
  }

  return autoBlackPoint;
}

/*!
  \brief Finds the most-exposed pixel that isn't saturated.

  Used with OffsetGainConvert to automatically set the offset.

  \sa OffsetGainConvert
*/

int ConverterBase::mostExposedValue(const QImage &image) {
  uint16_t autoWhitePoint = 0;

  auto *d = reinterpret_cast<const uint16_t *> (image.constBits());
  auto count = image.width() * image.height();

  for(int i = 0; i < count; ++i) {
    if(d[i] < 0xFFFF) {
      autoWhitePoint = std::max(d[i], autoWhitePoint);
    }
  }

  return autoWhitePoint;
}
