/*!
  \file offset_gain_convert.h
  \brief Definition of the OffsetGainConvert class.
*/

#ifndef OFFSET_GAIN_CONVERT_H
#define OFFSET_GAIN_CONVERT_H

#include "converter_base.h"

#include <QImage>

namespace Gray16 {
  /*!
    \brief OffsetGainConvert - Offset then bit-shift pixel values.

    This applies an offset to the pixel and then bit-shifts the value.

    finalPixelValue = (origPixelValue + offset) >> shift
    (outputValue is clamped 0 to 255);
  */

  class OffsetGainConvert : public ConverterBase {
  public:
    OffsetGainConvert() = default;

    enum class OffsetMode {
      Manual,
      LeastExposedOffset,
      MostExposedOffset
    };

    void setOffsetGain(int offset, int gain);
    void setOffsetMode(OffsetMode offsetMode);

    int detectedOffset() const;

  private:
    QImage mConvert(const QImage &originalImage) const override;

    OffsetMode mOffsetMode = OffsetMode::Manual; //!< Manual offsetting or automatic
    mutable int mOffset = 0; //!< Value to offset pixel value by first.
    int mGain = 0; //!< Bits to shift pixels value after offsetting.
  };
}

#endif // OFFSET_GAIN_CONVERT_H
