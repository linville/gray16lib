/*!
  \file false_color_convert.h
  \brief Definition of the ManualConvert class.
*/

#ifndef FALSE_COLOR_CONVERT_H
#define FALSE_COLOR_CONVERT_H

#include "converter_base.h"

#include <QImage>

namespace Gray16 {
  /*!
    \brief FalseColorConvert - Converts to a red/green/blue gradient.

    Current False Color Modes:
    \li Black Body
  */

  class FalseColorConvert : public ConverterBase {
  public:
    FalseColorConvert() = default;

    enum class FalseColorMode {
      BlackBody
    };

    void setFalseColorMode(FalseColorMode falseColorMode);

  private:
    QImage mConvert(const QImage &originalImage) const override;

    QImage mBlackBodyConvert(const QImage &originalImage) const;

    FalseColorMode mFalseColorMode;
  };
}

#endif // FALSE_COLOR_CONVERT_H
