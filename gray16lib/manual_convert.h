/*!
  \file manual_convert.h
  \brief Definition of the ManualConvert class.
*/

#ifndef MANUAL_CONVERT_H
#define MANUAL_CONVERT_H

#include "converter_base.h"

#include <QImage>

namespace Gray16 {
  /*!
    \brief ManualConvert - Set specific values for black point and white point.

    This scales the selected black-point and white-points down to 8-bit depth.
    As we can always display 8-bit depth, black-point should always be
    different by 255.

    scale = (whitePoint - blackPoint) / 255
    outputValue = (inputValue - blackPoint) / scale
    (outputValue is clamped 0 to 255);
  */

  class ManualConvert : public ConverterBase {
  public:
    ManualConvert() = default;
    void setExposurePoints(uint16_t blackPoint,
                           uint16_t whitePoint);

    static const uint16_t MaxBlackPoint = 65280;
    static const uint16_t MinWhitePoint = 255;

  private:
    QImage mConvert(const QImage &originalImage) const override;

    uint16_t mBlackPoint = 0; //!< Pixel value that anything smaller than or equal to is black.
    uint16_t mWhitePoint = 0xFFFF; //!< Pixel value that anything larger than or equal to is white.
  };
}

#endif // MANUAL_CONVERT_H
